//
// Created by 靖晏 on 2021/7/17.
//

#include "LeaderState.h"

namespace pod{

    bool LeaderState::HandleAppendRequest(json &AppendRe, string &r) {

        AppendRequest re = GetAppendRequest(AppendRe);
        AppendResponse ret;
        ret.id = context.GetId();
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
        else {
            appendFailed.emplace(re.id, re.index-1);
            appendFailed.emplace(re.id, re.index);
        }

        if(LogToCommit[re.index] > context.GetMemberNumber()/2)
        {
            context.SetCommitIndex(re.index);
            LogToCommit.erase(re.index);
            grape::Logger::INFO(format("Log %1% has been committed")%re.index);
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
        LogToCommit[re.entryIndex] = 1;
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

        //heartbeat 消息不会检查以下信息，设为0
        re.entryIndex = 0;
        re.lastEntryTerm = 0;
        re.lastEntryIndex = 0;
        re.commitIndex = 0;
        re.log = "";
        return CreateAppendRequest(re);
    }

    pair<int, string> LeaderState::replay() {
        if(appendFailed.empty())
            return {0, ""};
        auto replayLog = appendFailed.front();
        appendFailed.pop();
        AppendRequest re = context.AppendReplay(replayLog.second);
        string r = CreateAppendRequest(re);
        return {replayLog.first, r};
    }

}