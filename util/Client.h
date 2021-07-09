//
// Created by 靖晏 on 2021/7/7.
//

#ifndef RAFT_CLIENT_H
#define RAFT_CLIENT_H

#include "Session.h"
#include "Log.h"

using tcp = boost::asio::ip::tcp;

namespace grape{
    class Client{
    public:
        using callBackFunction = function<void(shared_ptr<Session> con)>;

        Client(boost::asio::io_context &ioc, const std::string& host, const std::string& port);

        Client(const Client&) =delete;
        Client(Client &) =delete;
        Client& operator=(Client &) =delete;
        Client& operator=(const Client &) =delete;

        void SetConnection(callBackFunction &con){
            connection_f = con;
        }

        void SetMessage(callBackFunction &msg){
            message_f = msg;
        }

        void SetClose(callBackFunction &cls){
            close_f = cls;
        }

    private:

        callBackFunction connection_f;
        callBackFunction message_f;
        callBackFunction close_f;

        tcp::socket socket_;
        tcp::resolver resolver_;

        void OnResolve(boost::system::error_code ec, tcp::resolver::results_type endpoints);

        void OnConnect(boost::system::error_code ec);
    };
}


#endif //RAFT_CLIENT_H
