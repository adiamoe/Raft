//
// Created by 靖晏 on 2021/7/13.
//

#include "RaftServer.h"

namespace pod{
    RaftServer::RaftServer(boost::asio::io_context &io_, int ID, vector<address> &member, int port):ioc(io_), server(ioc, port){
        for(auto & i : member){
            clients[i.id] = make_shared<grape::Client>(ioc);
            clients[i.id]->SetHostAndPort(i.host, i.port);

            clients[i.id]->SetConnection([this](const shared_ptr<grape::Session>& con) {
                grape::Logger::INFO(format("Raft client %1% receives response")%serverId);
            });

            clients[i.id]->SetMessage([this](const std::shared_ptr<grape::Session>& con) {
                string message = string(con->GetData().data());
                json js = json::parse(message);
                string ret;
                bool isNeedTransion = false;
                switch (js["type"].get<int>()) {
                    case APPEND_RESPONSE:
                        isNeedTransion = state->HandleAppendResponse(js, ret);
                        break;
                    case VOTE_RESPONSE:
                        isNeedTransion = state->HandleVoteResponse(js, ret);
                        break;
                    default:
                        grape::Logger::WARN(format("Unexpected message type in client!"));
                }
                if(!ret.empty()){
                    json js = json::parse(ret);
                    if(js["type"].get<int>() == TRANSFER && isNeedTransion){
                        Transfer ts = GetTransfer(js);
                        isNeedTransion = false;
                        if(ts.TransferToLeader)
                            Transition(ServerContext::LEADER);
                        else
                            Transition(ServerContext::FOLLOWER);
                    }
                }
                if(isNeedTransion){
                    Transition(ServerContext::FOLLOWER);
                }
            });

            clients[i.id]->SetClose([this](const std::shared_ptr<grape::Session>& con) {
                con->Shutdown();
            });
        }

        serverId = ID;
        state = new FollowerState(member.size(), serverId);
        timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::CANDIDATE);}, boost::asio::chrono::seconds(30));
        timer->Start();

        server.SetConnection([this](const shared_ptr<grape::Session>& con) {
            grape::Logger::INFO(format("Raft Server %1% receives message")%serverId);
        });

        server.SetMessage([this](const std::shared_ptr<grape::Session>& con) {
            string message = string(con->GetData().data());
            json js = json::parse(message);
            string ret;
            bool isNeedTransion = false;
            switch (js["type"].get<int>()) {
                case APPEND:
                    isNeedTransion = state->HandleAppendRequest(js, ret);
                    if(state->type() == ServerContext::FOLLOWER) {
                        timer->Cancle();
                        timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::CANDIDATE);}, boost::asio::chrono::seconds(40));
                        timer->Start();
                        grape::Logger::INFO(format("Follower timer reset!"));
                    }
                    con->DoWrite(ret);
                    break;
                case VOTE:
                    isNeedTransion = state->HandleVoteRequest(js, ret);
                    if(state->type() == ServerContext::FOLLOWER){
                        timer->Cancle();
                        timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::CANDIDATE);}, boost::asio::chrono::seconds(40));
                        timer->Start();
                        grape::Logger::INFO(format("Follower timer reset!"));
                    }
                    con->DoWrite(ret);
                    break;
                case COMMAND:
                    state->CommandRequest(js, ret);
                    if(state->type() == ServerContext::LEADER){
                        auto iter = clients.begin();
                        while(iter != clients.end()){
                            if(iter->first == serverId){
                                iter++;
                                continue;;
                            }
                            auto client = iter->second;
                            client->SetContext(ret);
                            client->Connect();
                            iter++;
                        }
                    } else {
                        if(state->GetLeaderId() != 0) {
                            auto client = clients[state->GetLeaderId()];
                            client->SetContext(message);
                            client->Connect();
                        }
                    }
                    break;
                default:
                    grape::Logger::WARN(format("Unexpected message type in server!"));
            }
            if(isNeedTransion){
                Transition(ServerContext::FOLLOWER);
            }
        });

        server.SetClose([this](const std::shared_ptr<grape::Session>& con) {
            con->Shutdown();
        });

        grape::Logger::INFO(format("Raft Node %1% get started")%serverId);
    }

    void RaftServer::Transition(ServerContext::STATE_TYPE type) {
        if(state->type() == type)
            return;

        grape::Logger::INFO(format("Raft Server %1% transitions from %2% to %3%")% serverId % States[state->type()] % States[type]);

        AbstractState *old = state;
        auto iter = clients.begin();
        switch(type){
            case ServerContext::FOLLOWER:
                state = new FollowerState(old->GetContext());
                timer->Cancle();
                timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::CANDIDATE);}, boost::asio::chrono::seconds(40));
                timer->Start();
                break;
            case ServerContext::CANDIDATE:
                state = new CandidateState(old->GetContext());
                timer->Cancle();
                timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::FOLLOWER);}, boost::asio::chrono::seconds(20));
                timer->Start();
                //异步client，似乎一个client会导致不能连接多个server
                while(iter != clients.end()){
                    if(iter->first == serverId) {
                        iter++;
                        continue;
                    }
                    auto client = iter->second;
                    string ret = state->pollRequest();
                    client->SetContext(ret);
                    client->Connect();
                    iter++;
                }
                break;
            case ServerContext::LEADER:
                state = new LeaderState(old->GetContext(), serverId);
                timer->Cancle();
                timer = std::make_shared<grape::Timer>(ioc, [this]{
                    auto iter = clients.begin();
                    while(iter != clients.end()){
                        if(iter->first == serverId) {
                            iter++;
                            continue;
                        }
                        auto client = iter->second;
                        string ret = state->HeartBeat();
                        client->SetContext(ret);
                        client->Connect();
                        iter++;
                        grape::Logger::DEBUG(format("Leader sends heartbeat to followers"));
                    }
                }, boost::asio::chrono::seconds(20));
                timer->Start();
        }
    }
}
