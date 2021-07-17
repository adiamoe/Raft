//
// Created by 靖晏 on 2021/7/17.
//

#ifndef RAFT_LEADERSTATE_H
#define RAFT_LEADERSTATE_H

#include "AbstractState.h"
#include <map>

namespace pod{
    class LeaderState : public AbstractState{
    private:
        map<int, int> LogToCommit;
    public:

        LeaderState(ServerContext context, int id): AbstractState(context){
            UpdateTermAndLeader(context.GetTerm(), id);
        }

        bool CommandRequest(json &command, string &r) override;

        bool HandleAppendRequest(json &AppendRe, string &r) override;

        bool HandleVoteRequest(json &VoteRe, string &r) override;

        bool HandleAppendResponse(json &AppendRe, string &r) override;

        bool HandleVoteResponse(json &VoteRe, string &r) override;

        ServerContext::STATE_TYPE type() override;

        string HeartBeat() override;

    };
}

#endif //RAFT_LEADERSTATE_H
