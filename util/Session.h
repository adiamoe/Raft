//
// Created by 靖晏 on 2021/7/7.
//

#ifndef RAFT_SESSION_H
#define RAFT_SESSION_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <queue>
#include <functional>
#include "Log.h"

using namespace std;
using tcp = boost::asio::ip::tcp;

enum ConnState{normal, overflow, time_error, read_error, write_error};
enum Mode {ready, writting, closed};

namespace grape {
    class Session : public enable_shared_from_this<Session> {
    public:
        using callBackFunction = function<void(shared_ptr<Session> con)>;

        Session(tcp::socket socket) : socket_(move(socket)), mode_(ready), state_(normal){}

        Session(const Session &) = delete;

        Session &operator=(const Session &) = delete;

        void Start();

        void SetConnection(const callBackFunction &con){
            connection_f = con;
        }

        void SetMessage(const callBackFunction &msg){
            message_f = msg;
        }

        void SetClose(const callBackFunction &cls){
            close_f = cls;
        }

        void DoWrite(const string &content);

        void Shutdown(){
            mode_ = closed;
        }

        array<char, 1024>& GetData(){
            return read_buffer_;
        }

        uint32_t GetLength(){
            return length_;
        }

    private:
        tcp::socket socket_;
        array<char, 1024> read_buffer_;
        queue<string> write_buffer_;
        uint32_t length_;
        Mode mode_;
        ConnState state_;
        callBackFunction connection_f;
        callBackFunction message_f;
        callBackFunction close_f;

        void DoReadLength();

        void DoReadData();

        void DoSend();

        void OnMessage(){
            if(message_f)
                message_f(shared_from_this());
        }

        void OnConnnection(){
            if(connection_f)
                connection_f(shared_from_this());
        }

        void OnClose(){
            if(close_f)
                close_f(shared_from_this());
        }

    };
}


#endif //RAFT_SESSION_H
