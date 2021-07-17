//
// Created by 靖晏 on 2021/7/17.
//

#include "LeaderState.h"

namespace pod{

    bool LeaderState::HandleAppendRequest(json &AppendRe, string &r) {

        AppendRequest re = GetAppendRequest(AppendRe);
        AppendResponse ret;
        ret.followerTerm = context.GetTerm();

        // step down as a Leader
        if(re.leaderTerm >= context.GetTerm()){
            context.SetTerm(re.leaderTerm);
            AbstractState::HandleAppendRequest(AppendRe, r);
            return true;
        }

        ret.index = re.entryIndex;
        ret.success = false;
        r = CreateAppendResponse(ret);
        return false;
    }

    bool LeaderState::HandleAppendResponse(json &AppendRe, string &r) {
        AppendResponse re = GetAppendResponse(AppendRe);
        if(UpdateTermAndLeader(re.followerTerm, 0))
            return true;

        if(re.index == -1)
            return false;

        if(re.success)
            LogToCommit[re.index]+=1;
        if(LogToCommit[re.index] > context.GetMemberNumber()/2)
        {
            context.SetCommitIndex(re.index);
            LogToCommit.erase(re.index);
        }
        return false;
    }

    bool LeaderState::HandleVoteRequest(json &VoteRe, string &r) {

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

    bool LeaderState::HandleVoteResponse(json &VoteRe, string &r) {

        VoteResponse re = GetVoteResponse(VoteRe);

        if(UpdateTermAndLeader(re.followerTerm, 0)){
            return true;
        }

        return false;
    }

    bool LeaderState::CommandRequest(json &command, string &r) {
        Command com = GetCommand(command);
        //Leader从来不会覆盖或删除自己的日志条目，对于新的记录，直接添加到Log里
        AppendRequest re = context.Append(com.log);
        LogToCommit[re.entryIndex] = 0;
        r = CreateAppendRequest(re);
        return false;
    }

    ServerContext::STATE_TYPE LeaderState::type() {
        return ServerContext::LEADER;
    }

    string LeaderState::HeartBeat() {
        AppendRequest re;
        re.leaderTerm = context.GetTerm();
        re.leaderID = context.GetLeader();
        re.heartBeat = true;
        return CreateAppendRequest(re);
    }

}