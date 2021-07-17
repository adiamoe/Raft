//
// Created by 靖晏 on 2021/7/13.
//

#include "RaftServer.h"

namespace pod{
    RaftServer::RaftServer(boost::asio::io_context &io_, int ID, vector<address> &member, int port):ioc(io_), server(ioc, port),
                                                                                     client(ioc){
        for(auto & i : member){
            members[i.id] = {i.host, i.port};
        }
        serverId = ID;
        state = new FollowerState(member.size(), serverId);
        timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::CANDIDATE);}, boost::asio::chrono::seconds(10));
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
                    if(state->type() == ServerContext::FOLLOWER)
                        timer->Reset();
                    con->DoWrite(ret);
                    break;
                case VOTE:
                    isNeedTransion = state->HandleVoteRequest(js, ret);
                    if(state->type() == ServerContext::FOLLOWER)
                        timer->Reset();
                    con->DoWrite(ret);
                    break;
                case COMMAND:
                    state->CommandRequest(js, ret);
                    if(state->type() == ServerContext::LEADER){
                        auto iter = members.begin();
                        while(iter != members.end() && iter->first != serverId){
                            string res = state->HeartBeat();
                            client.SetContext(res);
                            client.SetHostAndPort(iter->second.first, iter->second.second);
                            client.Connect();
                            iter++;
                        }
                    } else {
                        client.SetContext(message);
                        auto address = members[state->GetLeaderId()];
                        client.SetHostAndPort(address.first, address.second);
                        client.Connect();
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

        server.SetConnection([this](const shared_ptr<grape::Session>& con) {
            grape::Logger::INFO(format("Raft client %1% receives response")%serverId);
        });

        server.SetMessage([this](const std::shared_ptr<grape::Session>& con) {
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

        client.SetClose([this](const std::shared_ptr<grape::Session>& con) {
            con->Shutdown();
        });

        grape::Logger::INFO(format("Raft Node %1% get started")%serverId);
    }

    void RaftServer::Transition(ServerContext::STATE_TYPE type) {
        if(state->type() == type)
            return;

        grape::Logger::INFO(format("Raft Server %1% transitions from %2% to %3%")% serverId % States[state->type()] % States[type]);

        AbstractState *old = state;
        auto iter = members.begin();
        switch(type){
            case ServerContext::FOLLOWER:
                state = new FollowerState(old->GetContext());
                timer->Cancle();
                timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::CANDIDATE);}, boost::asio::chrono::seconds(10));
                timer->Start();
                break;
            case ServerContext::CANDIDATE:
                state = new CandidateState(old->GetContext());
                timer->Cancle();
                timer = std::make_shared<grape::Timer>(ioc, [this]{return Transition(ServerContext::FOLLOWER);}, boost::asio::chrono::seconds(10));
                timer->Start();
                while(iter != members.end() && iter->first != serverId){
                    string ret = state->pollRequest();
                    client.SetContext(ret);
                    client.SetHostAndPort(iter->second.first, iter->second.second);
                    client.Connect();
                    iter++;
                }
                break;
            case ServerContext::LEADER:
                state = new LeaderState(old->GetContext(), serverId);
                timer->Cancle();
                timer = std::make_shared<grape::Timer>(ioc, [this]{
                    auto iter = members.begin();
                    while(iter != members.end() && iter->first != serverId){
                        string ret = state->HeartBeat();
                        client.SetContext(ret);
                        client.SetHostAndPort(iter->second.first, iter->second.second);
                        client.Connect();
                        iter++;
                    }
                }, boost::asio::chrono::seconds(10));
        }
        delete old;
    }
}
