//
// Created by 靖晏 on 2021/7/18.
//

#include "../state/RaftServer.h"

int main(int argc, char *argv[]){

    if(argc != 2)
        return 0;

    string port = argv[1];

    boost::asio::io_context ioc;
    grape::Client client(ioc);
    client.SetHostAndPort("localhost", port);

    pod::Command com;
    com.log = "1234567890";
    client.SetContext(pod::CreateCommand(com));
    client.Connect();

    ioc.run();
    return 0;
}