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

        Client(boost::asio::io_context &ioc);

        Client(const Client&) =delete;
        Client(Client &) =delete;
        Client& operator=(Client &) =delete;
        Client& operator=(const Client &) =delete;

        void Connect();

        void SetConnection(const callBackFunction &con){
            connection_f = con;
        }

        void SetMessage(const callBackFunction &msg){
            message_f = msg;
        }

        void SetClose(const callBackFunction &cls){
            close_f = cls;
        }

        void SetHostAndPort(const string &Host, const string &Port){
            host = Host;
            port = Port;
        }

        void SetContext(const string &context){
            this->context = context;
        }

    private:

        callBackFunction connection_f;
        callBackFunction message_f;
        callBackFunction close_f;

        tcp::socket socket_;
        tcp::resolver resolver_;
        string host;
        string port;
        string context;

        void OnResolve(boost::system::error_code ec, tcp::resolver::results_type endpoints);

        void OnConnect(boost::system::error_code ec);
    };
}


#endif //RAFT_CLIENT_H
