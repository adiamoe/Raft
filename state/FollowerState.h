//
// Created by 靖晏 on 2021/7/12.
//

#ifndef RAFT_FOLLOWERSTATE_H
#define RAFT_FOLLOWERSTATE_H

#include <utility>

#include "AbstractState.h"
#include "CandidataState.h"

namespace pod{
    class FollowerState : public AbstractState{

    public:
        FollowerState(int member, int id): AbstractState(member, id) {}

        FollowerState(ServerContext context): AbstractState(context){}

        bool CommandRequest(json &command, string &r) override;

        bool HandleAppendRequest(json &AppendRe, string &r) override;

        bool HandleVoteRequest(json &VoteRe, string &r) override;

        bool HandleAppendResponse(json &AppendRe, string &r) override;

        bool HandleVoteResponse(json &VoteRe, string &r) override;

        ServerContext::STATE_TYPE type() override;

    };
}


#endif //RAFT_FOLLOWERSTATE_H
