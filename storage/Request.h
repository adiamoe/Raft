//
// Created by 靖晏 on 2021/7/11.
//

#ifndef RAFT_REQUEST_H
#define RAFT_REQUEST_H

#include "../jsonxx/json.hpp"

using json = jsonxx::json;

namespace pod{

    enum REQUEST_TYPE{
        APPEND = 1,             //leader向follower发起append request
        VOTE = 2,               //candidate向其他节点请求投票
        APPEND_RESPONSE = 3,    //append响应结果
        VOTE_RESPONSE = 4,      //投票响应结果
        COMMAND = 5,            //客户端请求
        TRANSFER = 6
    };

    struct Command{
        REQUEST_TYPE type = COMMAND;
        std::string log;
    };

    struct AppendRequest{
        REQUEST_TYPE type = APPEND;
        int leaderID;
        int leaderTerm;
        int entryIndex;
        int commitIndex;
        int lastEntryTerm;
        int lastEntryIndex;
        bool heartBeat;
        std::string log;
    };

    struct VoteRequest{
        REQUEST_TYPE type = VOTE;
        int candidateID;
        int candidateTerm;
        int lastEntryTerm;
        int lastEntryIndex;
    };

    struct AppendResponse{
        REQUEST_TYPE type = APPEND_RESPONSE;
        int id;
        int followerTerm;
        int index;
        bool success;
    };

    struct VoteResponse{
        REQUEST_TYPE type = VOTE_RESPONSE;
        int followerTerm;
        bool grant;
    };

    struct Transfer{
        REQUEST_TYPE type = TRANSFER;
        bool TransferToLeader;
    };

    std::string CreateAppendRequest(const AppendRequest& re);
    std::string CreateVoteRequest(const VoteRequest &re);
    std::string CreateAppendResponse(const AppendResponse &re);
    std::string CreateVoteResponse(const VoteResponse &re);
    std::string CreateCommand(const Command &co);
    std::string CreateTransfer(const Transfer &ts);

    AppendRequest GetAppendRequest(const json &js);
    VoteRequest GetVoteRequest(const json &js);
    AppendResponse GetAppendResponse(const json &js);
    VoteResponse GetVoteResponse(const json &js);
    Command GetCommand(const json &js);
    Transfer GetTransfer(const json &js);
}

#endif //RAFT_REQUEST_H
