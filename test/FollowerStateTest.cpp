//
// Created by 靖晏 on 2021/7/13.
//

#include "../state/RaftServer.h"

int main(){
    boost::asio::io_context ioc;
    vector<int> member{1,2};
    pod::RaftServer raftNode(ioc, 1, member);
    grape::Client client(ioc, "localhost", "8080");

    ioc.run();
    return 0;
}
