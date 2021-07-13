//
// Created by 靖晏 on 2021/7/12.
//

#include "FollowerState.h"

namespace pod{

    FollowerState::FollowerState(ServerContext &context) : AbstractState(context) {

    }

    void FollowerState::HandleAppendRequest(json &AppendRe) {

        AppendRequest re = GetAppendRequest(AppendRe);
        if(re.leaderTerm < context.GetTerm()){
            grape::Logger::WARN(format("Append Rejected: leader Term %1% < current Term %2%")
                                        % re.leaderTerm % context.GetTerm());
            return;
        }

        UpdateTermAndLeader(re.leaderTerm, re.leaderID);

        if(context.GetLeader() == re.leaderID){
            //取消定时器
            timer->reset();
            return;
        }

        int prevEntry = context.GetLogStore().LastIndex();
        int prevTerm = context.GetLogStore().Term(re.lastEntryIndex);
        if(re.lastEntryIndex > prevEntry){
            grape::Logger::WARN(format("Append Rejected: previous index %1% > local last index %2%")
                                        % re.lastEntryIndex % prevEntry);
            return;
        }
        if(re.lastEntryTerm != prevTerm){
            grape::Logger::WARN(format("Append Rejected: previous term %1% != local log Term in the previous entry %2%")
                                % re.lastEntryTerm % prevTerm);
            return;
        }

        size_t entrySize = context.GetLogStore().entrySize();
        if(re.entryIndex > entrySize){
            grape::Logger::WARN(format("Append Rejected: entry index %1% > local log current entry %2%")
                                % re.entryIndex % entrySize);

            return;
        }

        grape::Logger::INFO(format("Append entry into local log %1%") % re.entryIndex);
        context.GetLogStore().append(re.leaderTerm, re.entryIndex, re.log);
    }

    void FollowerState::HandleAppendResponse(json &AppendRe) {
        grape::Logger::WARN(format("Follower state not support append response!"));
    }

    void FollowerState::HandleVoteRequest(json &VoteRe) {

        VoteRequest re = GetVoteRequest(VoteRe);
        if(re.candidateTerm < context.GetTerm()){
            grape::Logger::WARN(format("Vote Rejected: candidate term %1% < local term %2%")
                                %re.candidateTerm % context.GetTerm());
            return;
        }

        UpdateTermAndLeader(re.candidateTerm, 0);

        if(context.GetLeader() != 0){
            grape::Logger::WARN(format("Vote Rejected: There can be only one leader in the same term!"));
            return;
        }

        if(context.GetLastVote() != 0){
            grape::Logger::WARN(format("Vote Rejected: Already voted for other candidate!"));
            return;
        }

        grape::Logger::INFO(format("Vote Granted: Vote for %1%")%re.candidateID);
    }

    void FollowerState::HandleVoteResponse(json &VoteRe) {
        grape::Logger::WARN(format("Follower state not support vote response!"));
    }

    AbstractState::STATE_TYPE FollowerState::type() {
        return FOLLOWER;
    }

}