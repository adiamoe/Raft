//
// Created by 靖晏 on 2021/7/12.
//

#ifndef RAFT_FOLLOWERSTATE_H
#define RAFT_FOLLOWERSTATE_H

#include "AbstractState.h"

namespace pod{
    class FollowerState : public AbstractState{

    public:
        FollowerState(ServerContext &context);

        void CommandRequest(json &command) override;

        void HandleAppendRequest(json &AppendRe) override;

        void HandleVoteRequest(json &VoteRe) override;

        void HandleAppendResponse(json &AppendRe) override;

        void HandleVoteResponse(json &VoteRe) override;

        AbstractState::STATE_TYPE type() override;

    };
}


#endif //RAFT_FOLLOWERSTATE_H
