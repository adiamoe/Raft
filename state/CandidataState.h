//
// Created by 靖晏 on 2021/7/14.
//

#ifndef RAFT_CANDIDATASTATE_H
#define RAFT_CANDIDATASTATE_H

#include <utility>

#include "AbstractState.h"

namespace pod{
    class CandidateState : public AbstractState{
    private:
        int vote;
    public:
        CandidateState(ServerContext context): AbstractState(context), vote(0){
            UpdateTermAndLeader(context.GetTerm()+1, 0);
        }

        string pollRequest() override;

        bool CommandRequest(json &command, string &r) override;

        bool HandleAppendRequest(json &AppendRe, string &r) override;

        bool HandleVoteRequest(json &VoteRe, string &r) override;

        bool HandleAppendResponse(json &AppendRe, string &r) override;

        bool HandleVoteResponse(json &VoteRe, string &r) override;

        ServerContext::STATE_TYPE type() override;
    };
}


#endif //RAFT_CANDIDATASTATE_H
