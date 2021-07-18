//
// Created by 靖晏 on 2021/7/8.
//

#ifndef RAFT_LOG_H
#define RAFT_LOG_H

#include <string>
#include <boost/format.hpp>

using boost::format;

namespace grape{
    class Logger{
    public:
        static void INFO(const format &msg);
        static void WARN(const format &msg);
        static void ERROR(const format &msg);
        static void DEBUG(const format &msg);
    };
}

#endif //RAFT_LOG_H
