//
// Created by 靖晏 on 2021/7/9.
//

#include "ServerContext.h"

namespace pod{
    void ServerContext::Append(int appendTerm, int index, long log) {
        if(appendTerm > term){
            grape::Logger::INFO("Append Request Term " + std::to_string(appendTerm) + " is larger, convert to it");
        }
        if(appendTerm < term){
            grape::Logger::WARN("Append Request Term " + std::to_string(appendTerm)
                                + " < Current Term " + std::to_string(term));
            return;
        }
        if(index < commitIndex){
            grape::Logger::WARN("Append Index " + std::to_string(index)
                                + " < Commit Index "+ std::to_string(commitIndex));
            return;
        }

        //-1为空AppendRequest，作为心跳
        if(log != -1)
            store.append(index, log);
    }
}
