//
// Created by 靖晏 on 2021/7/7.
//

#include "Client.h"
#include "../storage/Request.h"

namespace grape{
    Client::Client(boost::asio::io_context &ioc): socket_(ioc), resolver_(ioc), retryTime(0) {}

    void Client::Connect(bool reply) {
        resolver_.async_resolve(tcp::v4(), host, port,
                                bind(&Client::OnResolve, this, placeholders::_1, placeholders::_2, reply));
    }

    void Client::OnResolve(boost::system::error_code ec, tcp::resolver::results_type endpoints, bool reply){
        if(!ec){
            Logger::INFO(format("Resolve Succeed"));
            boost::asio::async_connect(socket_, endpoints, bind(&Client::OnConnect, this, placeholders::_1, reply));
        }
        else{
            Logger::WARN(format("Resolve Failed"));
        }
    }

    void Client::OnConnect(boost::system::error_code ec, bool reply) {
        if(!ec){
            retryTime = 0;
            auto se = make_shared<Session>(move(socket_));
            se->SetConnection(connection_f);
            se->SetMessage(message_f);
            se->SetClose(close_f);
            Logger::INFO(format("Connect Succeed"));

            se->DoWrite(context);
            if(reply)
                se->Start();
        }
        else {
            retryTime++;
            if(retryTime<=3){
                Logger::WARN(format("Connect Failed, try to reconnect"));
                Connect(reply);
            } else{
                Logger::WARN(format("Three failures. Abandon the connection"));
            }
        }
    }

}
