//
// Created by 靖晏 on 2021/7/18.
//

#include "../state/RaftServer.h"
#include <iostream>

int main(int argc, char *argv[]){

    if(argc != 2)
        return 0;

    string port = argv[1];

    boost::asio::io_context ioc;
    grape::Client client(ioc);
    client.SetHostAndPort("localhost", port);

    pod::Command com;
    string log;
    while(std::cin>>log){
        com.log = log;
        client.SetContext(pod::CreateCommand(com));
        client.Connect();
        ioc.run();
    }

    return 0;
}