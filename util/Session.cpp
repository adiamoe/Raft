//
// Created by 靖晏 on 2021/7/7.
//

#include "Session.h"
#include "util.h"

namespace grape{
    void Session::Start()
    {
        OnConnnection();
        DoReadLength();
    }

    void Session::DoReadLength() {
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(&length_, sizeof(length_)),
                                [self](boost::system::error_code ec, size_t length){
        if(!ec){
            self->length_ = NetworkToHost(self->length_);
            if(self->length_  > self->read_buffer_.size()){
                Logger::WARN(format("overflow"));
                self->OnClose();
            }
            else{
                self->DoReadData();
            }
        }
        else{
            if(ec == boost::asio::error::eof)
                Logger::INFO(format("Connection close"));
            else{
                Logger::WARN(format("read_error"));
            }
            self->OnClose();
        }
        });
    }

    void Session::DoReadData() {
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_buffer_, length_),
                                [self](boost::system::error_code ec, size_t length){
            if(!ec)
            {
                Logger::INFO(format("Read message %1%") % self->read_buffer_.data());
                self->OnMessage();
                self->DoReadLength();
            }
            else{
                if(ec == boost::asio::error::eof)
                    Logger::INFO(format("Connection close"));
                else{
                    Logger::WARN(format("read_error"));
                }
                self->OnClose();
            }
        });
    }

    //

    void Session::DoWrite(const string &content) {
        uint32_t len = content.size();
        len = HostToNetwork(len);
        write_buffer_.push(std::string(reinterpret_cast<const char*>(&len), sizeof(len))+content);
        if(mode_ == writting)
            return;
        else{
            mode_ = writting;
            DoSend();
        }
    }

    void Session::DoSend() {
        if(mode_ != writting)
            return;
        auto self(shared_from_this());
        if(write_buffer_.empty()){
            mode_ = ready;
            return;
        }
        else{
            string to_send(move(write_buffer_.front()));
            write_buffer_.pop();
            boost::asio::async_write(socket_, boost::asio::buffer(to_send, to_send.size()),
                                     [self](boost::system::error_code ec, size_t length){
               if(!ec){
                   self->DoSend();
               }
               else {
                   Logger::WARN(format("write_error"));
               }
            });
        }
    }
}