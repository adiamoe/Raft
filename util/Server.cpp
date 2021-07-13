//
// Created by 靖晏 on 2021/7/7.
//

#include "Server.h"

namespace grape{
    Server::Server(boost::asio::io_context &ioc, uint16_t port) : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)){
        DoAccept();
    }

    void Server::DoAccept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket){
            if(!ec)
            {
                auto se = make_shared<Session>(move(socket));
                se->SetConnection(connection_f);
                se->SetMessage(message_f);
                se->SetClose(close_f);
                se->Start();
            }
            else{
                Logger::WARN(format("Accept Failed"));
            }
            DoAccept();
        });
    }
}
