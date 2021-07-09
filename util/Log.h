//
// Created by 靖晏 on 2021/7/8.
//

#ifndef RAFT_LOG_H
#define RAFT_LOG_H

#include <string>

namespace grape{
    class Logger{
    public:
        static void INFO(const std::string &msg);
        static void WARN(const std::string &msg);
        static void ERROR(const std::string &message);
    };
}

#endif //RAFT_LOG_H
