//
// Created by 靖晏 on 2021/7/9.
//

#ifndef RAFT_SERVERCONTEXT_H
#define RAFT_SERVERCONTEXT_H
#include "../storage/LogStore.h"
#include "../util/Log.h"

namespace pod{
    class ServerContext{
    private:
        int ServerId;

        volatile int leader;
        volatile int term;
        volatile int commitIndex;

        LogStore store;

    public:
        void Append(int appendTerm, int index, long log);

        int GetID(){
            return ServerId;
        }

        void SetTerm(int newTerm){
            assert(term<newTerm);
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

    };
}


#endif //RAFT_SERVERCONTEXT_H
