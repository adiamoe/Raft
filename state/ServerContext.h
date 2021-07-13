//
// Created by 靖晏 on 2021/7/9.
//

#ifndef RAFT_SERVERCONTEXT_H
#define RAFT_SERVERCONTEXT_H

#include "../util/Client.h"
#include "../storage/LogStore.h"
#include "../util/Log.h"
#include "../storage/Request.h"
#include <vector>

namespace pod{
    class ServerContext{
    private:

        std::vector<int> member;
        std::vector<grape::Client> sender;

        int ServerId;

        volatile int leader;
        volatile int term;
        volatile int commitIndex;
        volatile int lastVote;

        LogStore store;

    public:

        ServerContext():leader(0), term(0), commitIndex(-1), lastVote(0){}

        int GetID(){
            return ServerId;
        }

        void SetTerm(int newTerm){
            assert(term <= newTerm);
            term = newTerm;
        }

        void SetLeader(int newLeader){
            leader = newLeader;
        }

        int GetLeader(){
            return leader;
        }

        int GetTerm(){
            return term;
        }

        int GetLastVote(){
            return lastVote;
        }

        LogStore& GetLogStore(){
            return store;
        }

    };
}


#endif //RAFT_SERVERCONTEXT_H
