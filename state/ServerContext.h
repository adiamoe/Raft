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

class AbstractState;

namespace pod{
    class ServerContext{
    private:

        volatile int leader;
        volatile int term;
        volatile int commitIndex;
        volatile int lastVote;

        int memberNum;
        int serverId;

        LogStore store;
        AbstractState *state;

    public:

        enum STATE_TYPE{
            FOLLOWER,
            CANDIDATE,
            LEADER
        };

        ServerContext(ServerContext &context) = default;

        ServerContext(int member, int id):memberNum(member), serverId(id), leader(0), term(0), commitIndex(-1), lastVote(0){}

        void SetCommitIndex(int index){
            commitIndex = index;
        }

        void SetTerm(int newTerm){
            if(term <= newTerm){
                term = newTerm;
                leader = 0;
                lastVote = 0;
            }
        }

        void SetLeader(int newLeader){
            leader = newLeader;
            lastVote = 0;
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

        void SetVoteFor(int candidateId){
            lastVote = candidateId;
        }

        LogStore& GetLogStore(){
            return store;
        }

        int GetMemberNumber(){
            return memberNum;
        }

        AppendRequest Append(string &log);

        VoteRequest Vote();

    };
}


#endif //RAFT_SERVERCONTEXT_H
