//
// Created by 靖晏 on 2021/7/17.
//

#include "../state/RaftServer.h"

int main(int argc, char *argv[]){

    int id = *argv[1];
    int port = *argv[2];
    boost::asio::io_context ioc;
    vector<pod::address> member = {{"localhost", "8080", 1}, {"localhost", "8081", 2}, {"localhost", "8082", 3}};
    pod::RaftServer raftNode1(ioc, id, member, port);
    grape::Client client(ioc);
    client.SetHostAndPort("localhost", "8080");

    pod::Command com;
    com.log = "1234567890";
    client.SetContext(pod::CreateCommand(com));
    client.Connect();

    ioc.run();
    return 0;
}