//
// Created by 靖晏 on 2021/7/9.
//

#ifndef RAFT_ABSTRACTSTATE_H
#define RAFT_ABSTRACTSTATE_H

#include "ServerContext.h"
#include "../util/timer.h"
#include "../jsonxx/json.hpp"

using json = jsonxx::json;

using std::string;

namespace pod{
    class AbstractState{
    protected:
        ServerContext context;
        grape::Timer *timer;
    public:

        enum STATE_TYPE{
            FOLLOWER,
            CANDIDATE,
            LEADER
        };

        AbstractState(ServerContext &context):context(context){
            timer = nullptr;
        }

        void HandleClientRequest(string &to_json){
            json js = json::parse(to_json);
            switch (js["type"].get<int>()) {
                case APPEND:
                    HandleAppendRequest(js);
                    break;
                case VOTE:
                    HandleVoteRequest(js);
                    break;
                case APPEND_RESPONSE:
                    HandleAppendResponse(js);
                    break;
                case VOTE_RESPONSE:
                    HandleVoteResponse(js);
                    break;
                case COMMAND:
                    CommandRequest(js);
                    break;
            }
        }

        virtual void CommandRequest(json &command) = 0;

        virtual void HandleAppendRequest(json &AppendRe) = 0;

        virtual void HandleVoteRequest(json &VoteRe) = 0;

        virtual void HandleAppendResponse(json &AppendRe) = 0;

        virtual void HandleVoteResponse(json &VoteRe) = 0;

        virtual STATE_TYPE type() = 0;

        bool UpdateTermAndLeader(int term, int leader);

    };
}


#endif //RAFT_ABSTRACTSTATE_H
