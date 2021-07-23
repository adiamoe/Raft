//
// Created by 靖晏 on 2021/7/9.
//

#include "AbstractState.h"

namespace pod{
    bool AbstractState::UpdateTermAndLeader(int term, int leader) {
        if(term > context.GetTerm() || (term == context.GetTerm() && context.GetLeader() == 0 && leader != 0 )){
            grape::Logger::INFO(format("Update term and leader new config: %1%, %2%, old config: %3%, %4%")
                                        %term%leader%context.GetTerm()%context.GetLeader());

            context.SetTerm(term);
            context.SetLeader(leader);
            return true;
        }
        return false;
    }

    bool AbstractState::HandleAppendRequest(json &AppendRe, string &r) {
        AppendRequest re = GetAppendRequest(AppendRe);
        AppendResponse ret;
        ret.id = context.GetId();
        ret.followerTerm = context.GetTerm();
        ret.index = re.heartBeat? -1:re.entryIndex;

        if(re.leaderTerm < context.GetTerm()){
            grape::Logger::WARN(format("Append Rejected: leader Term %1% < current Term %2%")
                                % re.leaderTerm % context.GetTerm());
            ret.success = false;
            r = CreateAppendResponse(ret);
            return false;
        }

        UpdateTermAndLeader(re.leaderTerm, re.leaderID);

        // Term may be updated
        ret.followerTerm = context.GetTerm();

        if(context.GetLeader() == re.leaderID){
            if(re.heartBeat){
                ret.success = true;
                r =  CreateAppendResponse(ret);
                return false;
            }
        }

        size_t entrySize = context.GetLogStore().entrySize();
        if(re.entryIndex > entrySize){
            grape::Logger::WARN(format("Append Rejected: entry index %1% > local log current entry %2%")
                                % re.entryIndex % entrySize);
            ret.success = false;
            r = CreateAppendResponse(ret);
            return false;
        }

        int prevEntry = context.GetLogStore().LastIndex();
        int prevTerm = context.GetLogStore().Term(re.lastEntryIndex);
        if(re.lastEntryIndex > prevEntry){
            grape::Logger::WARN(format("Append Rejected: previous index %1% > local last index %2%")
                                % re.lastEntryIndex % prevEntry);
            ret.success = false;
            r =  CreateAppendResponse(ret);
            return false;
        }
        if(re.lastEntryTerm != prevTerm){
            grape::Logger::WARN(format("Append Rejected: previous term %1% != local log Term in the previous entry %2%")
                                % re.lastEntryTerm % prevTerm);
            ret.success = false;
            r = CreateAppendResponse(ret);
            return false;
        }

        grape::Logger::INFO(format("Append entry into local log %1%") % re.entryIndex);
        context.GetLogStore().append(re.leaderTerm, re.entryIndex, re.log);

        ret.success = true;
        r =  CreateAppendResponse(ret);
        return false;
    }

    bool AbstractState::HandleVoteRequest(json &VoteRe, string &r) {
        VoteRequest re = GetVoteRequest(VoteRe);
        VoteResponse ret;
        ret.followerTerm = context.GetTerm();

        if(re.candidateTerm < context.GetTerm()){
            grape::Logger::WARN(format("Vote Rejected: candidate term %1% < local term %2%")
                                %re.candidateTerm % context.GetTerm());
            ret.grant = false;
            r = CreateVoteResponse(ret);
            return false;
        }

        UpdateTermAndLeader(re.candidateTerm, 0);
        ret.followerTerm = context.GetTerm();

        if(context.GetLeader() != 0){
            grape::Logger::WARN(format("Vote Rejected: There can be only one leader in the same term!"));
            ret.grant = false;
            r =  CreateVoteResponse(ret);
            return false;
        }

        if(context.GetLastVote() != 0){
            grape::Logger::WARN(format("Vote Rejected: Already voted for other candidate!"));
            ret.grant = false;
            r =  CreateVoteResponse(ret);
            return false;
        }

        grape::Logger::INFO(format("Vote Granted: Vote for %1%")%re.candidateID);
        ret.grant = true;
        context.SetVoteFor(re.candidateID);
        r =  CreateVoteResponse(ret);
        return false;
    }
}