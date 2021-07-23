//
// Created by 靖晏 on 2021/7/13.
//

#ifndef RAFT_RAFTSERVER_H
#define RAFT_RAFTSERVER_H

#include "../util/Client.h"
#include "../util/Server.h"
#include "FollowerState.h"
#include "CandidataState.h"
#include "LeaderState.h"

namespace pod{

    static string States[3] = {"Follower", "Candidate", "Leader"};

    struct address{
        string host;
        string port;
        int id;
    };

    class RaftServer {
    public:
        RaftServer(boost::asio::io_context &io_, int ID, vector<address> &member, int port);
    private:
        void Transition(ServerContext::STATE_TYPE type);

        void poll(const string &msg);

        unsigned int getRandom() const;

        boost::asio::io_context &ioc;
        AbstractState *state;
        grape::Server server;
        map<int, shared_ptr<grape::Client>> clients;
        int serverId;
        shared_ptr<grape::Timer> timer;
    };
}


#endif //RAFT_RAFTSERVER_H
