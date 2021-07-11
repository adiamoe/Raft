//
// Created by 靖晏 on 2021/7/9.
//

#ifndef RAFT_ABSTRACTSTATE_H
#define RAFT_ABSTRACTSTATE_H
#include "ServerContext.h"
#include "../jsonxx/json.hpp"

using json = jsonxx::json;

namespace pod{
    class AbstractState{
    private:
        ServerContext context;
    public:
        AbstractState(ServerContext &context){
            this->context = context;
        }

        virtual bool command(json );

    };
}


#endif //RAFT_ABSTRACTSTATE_H
