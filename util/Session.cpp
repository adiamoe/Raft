//
// Created by 靖晏 on 2021/7/7.
//

#include "Session.h"
#include "util.h"
#include <iostream>

namespace grape{
    void Session::Start()
    {
        OnConnnection();
        if(state_ != normal)
            OnClose();
        DoReadLength();
    }

    void Session::DoReadLength() {
        if(mode_ == closed)
            return;
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(&length_, sizeof(length_)),
                                [self](boost::system::error_code ec, size_t length){
        if(!ec){
            self->length_ = NetworkToHost(self->length_);
            if(self->length_  > self->read_buffer_.size()){
                self->state_ = overflow;
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
                self->state_ = read_error;
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
                if(self->state_ == normal)
                    self->DoReadLength();
                else
                    self->OnClose();
            }
            else{
                if(ec == boost::asio::error::eof)
                    Logger::INFO(format("Connection close"));
                else{
                    self->state_ = read_error;
                    Logger::WARN(format("read_error"));
                }
                self->OnClose();
            }
        });
    }

    //

    void Session::DoWrite(const string &content) {
        if(mode_ == closed)
            return;
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
            //OnWriteComplete();
            if(state_ != normal)
                OnClose();
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
                   self->state_ = write_error;
                   Logger::WARN(format("write_error"));
                   self->OnClose();
               }
            });
        }
    }
}