//
// Created by 靖晏 on 2021/7/9.
//

#ifndef RAFT_ABSTRACTSTATE_H
#define RAFT_ABSTRACTSTATE_H

#include <boost/asio.hpp>
#include "ServerContext.h"
#include "../util/timer.h"
#include "../jsonxx/json.hpp"

using json = jsonxx::json;

using std::string;

namespace pod{
    class AbstractState{
    protected:
        ServerContext context;
    public:

        AbstractState(int member, int id): context(member, id){}

        AbstractState(ServerContext context):context(context){}

        ServerContext& GetContext(){
            return context;
        }

        virtual bool CommandRequest(json &command, string &r) = 0;

        virtual bool HandleAppendRequest(json &AppendRe, string &r);

        virtual bool HandleVoteRequest(json &VoteRe, string &r);

        virtual bool HandleAppendResponse(json &AppendRe, string &r) = 0;

        virtual bool HandleVoteResponse(json &VoteRe, string &r) = 0;

        virtual ServerContext::STATE_TYPE type() = 0;

        virtual string HeartBeat(){
            grape::Logger::ERROR(format("not support!"));
        }

        virtual string pollRequest(){
            grape::Logger::ERROR(format("not support!"));
        }

        virtual pair<int, string> replay(){
            grape::Logger::ERROR(format("not support!"));
        }

        int GetLeaderId(){
            return context.GetLeader();
        }

        bool UpdateTermAndLeader(int term, int leader);

    };
}


#endif //RAFT_ABSTRACTSTATE_H
