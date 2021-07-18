//
// Created by 靖晏 on 2021/7/17.
//

#include "../state/RaftServer.h"

int main(int argc, char *argv[]){

    if(argc!=3)
        return 0;

    boost::asio::io_context ioc;

    int id = atoi(argv[1]);
    int port = atoi(argv[2]);
    vector<pod::address> member = {{"localhost", "50000", 1}, {"localhost", "50001", 2}, {"localhost", "50002", 3}};
    pod::RaftServer raftNode1(ioc, id, member, port);

    ioc.run();
    return 0;
}