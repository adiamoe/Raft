//
// Created by 靖晏 on 2021/7/12.
//

#include "FollowerState.h"

namespace pod{

    bool FollowerState::HandleAppendRequest(json &AppendRe, string &r) {
        return AbstractState::HandleAppendRequest(AppendRe, r);
    }

    bool FollowerState::HandleAppendResponse(json &AppendRe, string &r) {
        grape::Logger::WARN(format("Follower state not support append response!"));
    }

    bool FollowerState::HandleVoteRequest(json &VoteRe, string &r) {
        return AbstractState::HandleVoteRequest(VoteRe, r);
    }

    bool FollowerState::HandleVoteResponse(json &VoteRe, string &r) {
        grape::Logger::WARN(format("Follower state not support vote response!"));
        return false;
    }

    bool FollowerState::CommandRequest(json &command, string &r) {
        grape::Logger::WARN(format("Follower state not support command Request!"));
        return false;
    }

    ServerContext::STATE_TYPE FollowerState::type() {
        return ServerContext::FOLLOWER;
    }

}