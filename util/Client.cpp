//
// Created by 靖晏 on 2021/7/7.
//

#include "Client.h"

namespace grape{
    Client::Client(boost::asio::io_context &ioc, const std::string &host, const std::string &port): socket_(ioc), resolver_(ioc) {
        resolver_.async_resolve(tcp::v4(), host, port,
                                bind(&Client::OnResolve, this, placeholders::_1, placeholders::_2));
    }

    void Client::OnResolve(boost::system::error_code ec, tcp::resolver::results_type endpoints){
        if(!ec){
            Logger::INFO(format("Resolve Succeed"));
            boost::asio::async_connect(socket_, endpoints, bind(&Client::OnConnect, this, placeholders::_1));
        }
        else{
            Logger::WARN(format("Resolve Failed"));
        }
    }

    void Client::OnConnect(boost::system::error_code ec) {
        if(!ec){
            auto se = make_shared<Session>(move(socket_));
            se->SetMessage(message_f);
            se->SetConnection(connection_f);
            se->SetClose(close_f);
            Logger::INFO(format("Connect Succeed"));
            se->DoWrite("Hello World!");
            se->Start();
        }
        else {
            Logger::WARN(format("Connect Failed"));
        }
    }

}
