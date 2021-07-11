//
// Created by 靖晏 on 2021/7/11.
//

#include "Request.h"

namespace pod{

    std::string CreateAppendRequest(const AppendRequest& re){
        json js;
        js["type"] = int(APPEND);
        js["leaderID"] = re.leaderID;
        js["leaderTerm"] = re.leaderTerm;
        js["commitIndex"] = re.commitIndex;
        js["lastEntryTerm"] = re.lastEntryTerm;
        js["lastEntryIndex"] = re.lastEntryIndex;
        js["heartBeat"] = re.heartBeat;
        js["log"] = re.log;
        return js.dump();
    }

    std::string CreateVoteRequest(const VoteRequest &re){
        json js;
        js["type"] = int(VOTE);
        js["candidateID"] = re.candidateID;
        js["candidateTerm"] = re.candidateTerm;
        js["lastEntryTerm"] = re.lastEntryTerm;
        js["lastEntryIndex"] = re.lastEntryIndex;
        return js.dump();
    }

    std::string CreateAppendResponse(const AppendResponse &re){
        json js;
        js["type"] = int(APPEND_RESPONSE);
        js["seccess"] = re.success;
        return js.dump();
    }

    std::string CreateVoteResponse(const VoteResponse &re){
        json js;
        js["type"] = int(VOTE_RESPONSE);
        js["grant"] = re.grant;
        return js.dump();
    }

    std::string CreateCommand(const Command &co){
        json js;
        js["type"] = int(COMMAND);
        js["log"] = co.log;
        return js.dump();
    }

    AppendRequest GetAppendRequest(const json &js){
        AppendRequest re;
        re.type = (REQUEST_TYPE) js["type"].get<int>();
        re.leaderID = js["leaderID"].get<int>();
        re.leaderTerm = js["leaderTerm"].get<int>();
        re.commitIndex = js["commitIndex"].get<int>();
        re.lastEntryTerm = js["lastEntryTerm"].get<int>();
        re.lastEntryIndex = js["lastEntryIndex"].get<int>();
        re.heartBeat = js["heartBeat"].get<bool>();
        re.log = js["log"].get<std::string>();
        return re;
    }

    VoteRequest GetVoteRequest(const json &js){
        VoteRequest re;
        re.type = (REQUEST_TYPE) js["type"].get<int>();
        re.candidateID = js["candidateID"].get<int>();
        re.candidateTerm = js["candidateTerm"].get<int>();
        re.lastEntryTerm = js["lastEntryTerm"].get<int>();
        re.lastEntryIndex = js["lastEntryIndex"].get<int>();
        return re;
    }

    AppendResponse GetAppendResponse(const json &js){
        AppendResponse re;
        re.type = (REQUEST_TYPE) js["type"].get<int>();
        re.success = js["success"].get<bool>();
        return re;
    }

    VoteResponse GetVoteResponse(const json &js){
        VoteResponse re;
        re.type = (REQUEST_TYPE) js["type"].get<int>();
        re.grant = js["grant"].get<bool>();
        return re;
    }

    Command GetCommand(const json &js){
        Command re;
        re.type = (REQUEST_TYPE) js["type"].get<int>();
        re.log = js["log"].get<std::string>();
        return re;
    }

}

