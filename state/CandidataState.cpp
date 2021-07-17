//
// Created by 靖晏 on 2021/7/14.
//

#include "CandidataState.h"

namespace pod{

    bool CandidateState::HandleAppendRequest(json &AppendRe, string &r) {

        AppendRequest re = GetAppendRequest(AppendRe);
        AppendResponse ret;
        ret.followerTerm = context.GetTerm();

        // step down as a candidate
        if(re.leaderTerm >= context.GetTerm()){
            context.SetTerm(re.leaderTerm);
            AbstractState::HandleAppendRequest(AppendRe, r);
            return true;
        }

        return false;
    }

    bool CandidateState::HandleAppendResponse(json &AppendRe, string &r) {
        grape::Logger::INFO(format("Candidate state not support append response!"));
        return false;
    }

    bool CandidateState::HandleVoteRequest(json &VoteRe, string &r) {

        VoteRequest re = GetVoteRequest(VoteRe);

        if(UpdateTermAndLeader(re.candidateTerm, 0)){
            AbstractState::HandleVoteRequest(VoteRe, r);
            return true;
        }

        VoteResponse ret;
        ret.followerTerm = context.GetTerm();
        ret.grant = false;
        r = CreateVoteResponse(ret);
        return false;
    }

    bool CandidateState::HandleVoteResponse(json &VoteRe, string &r) {

        VoteResponse re = GetVoteResponse(VoteRe);

        if(UpdateTermAndLeader(re.followerTerm, 0)){
            Transfer ts;
            ts.TransferToLeader = false;
            r = CreateTransfer(ts);
            return true;
        }

        if(re.grant)
            vote++;
        if(vote > context.GetMemberNumber()/2)
        {
            Transfer ts;
            ts.TransferToLeader = true;
            r = CreateTransfer(ts);
            return true;
        }
        return false;
    }

    bool CandidateState::CommandRequest(json &command, string &r) {
        grape::Logger::WARN(format("Candidate state not support command Request!"));
        return false;
    }

    ServerContext::STATE_TYPE CandidateState::type() {
        return ServerContext::CANDIDATE;
    }

    string CandidateState::pollRequest(){
        VoteRequest re = context.Vote();
        return CreateVoteRequest(re);
    }

}