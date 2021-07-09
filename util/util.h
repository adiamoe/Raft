//
// Created by 靖晏 on 2021/7/7.
//

#ifndef RAFT_UTIL_H
#define RAFT_UTIL_H
#include <arpa/inet.h>
#include <string>

namespace grape{

    std::string GetIpFromIport(const std::string& iport)
    {
        auto iter = iport.find_first_of(':');
        return iport.substr(0, iter);
    }

    std::string GetPortFromIport(const std::string& iport)
    {
        auto iter = iport.find_first_of(':');
        return iport.substr(iter, iport.size() - iter);
    }

    uint16_t HostToNetwork(uint16_t n) {
        return htons(n);
    }

    uint16_t NetworkToHost(uint16_t n) {
        return ntohs(n);
    }

    uint32_t HostToNetwork(uint32_t n) {
        return htonl(n);
    }

    uint32_t NetworkToHost(uint32_t n) {
        return ntohl(n);
    }

}

#endif //RAFT_UTIL_H
