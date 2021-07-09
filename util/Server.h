//
// Created by 靖晏 on 2021/7/7.
//

#ifndef RAFT_SERVER_H
#define RAFT_SERVER_H

#include <boost/asio.hpp>
#include "Session.h"

using tcp = boost::asio::ip::tcp;

namespace grape{
    class Server{
    public:
        using callBackFunction = function<void(shared_ptr<Session> con)>;

        Server(boost::asio::io_context& ioc, uint16_t port);

        Server(const Server&) = delete;
        Server& operator=(const Server&) =delete;

        void SetConnection(const callBackFunction &con){
            connection_f = con;
        }

        void SetMessage(const callBackFunction &msg){
            message_f = msg;
        }

        void SetClose(const callBackFunction &cls){
            close_f = cls;
        }

    private:

        void DoAccept();

        callBackFunction connection_f;
        callBackFunction message_f;
        callBackFunction close_f;

        tcp::acceptor acceptor_;
    };
}


#endif //RAFT_SERVER_H
