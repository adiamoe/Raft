//
// Created by 靖晏 on 2021/7/12.
//

#include "../util/Log.h"
#include <string>

int main()
{
    std::string State[3] = {"Follower", "Candidate", "Leader"};
    enum STATE_TYPE{
        FOLLOWER,
        CANDIDATE,
        LEADER
    };
    grape::Logger::INFO(format("This is %1% message for %2% format")%1%State[FOLLOWER]);

    return 0;
}

