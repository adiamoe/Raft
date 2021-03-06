//
// Created by 靖晏 on 2021/7/10.
//

#ifndef RAFT_TIMER_H
#define RAFT_TIMER_H

#include <boost/asio.hpp>
#include <memory>
#include <functional>

namespace grape{
class Timer: public std::enable_shared_from_this<Timer> {
    public:
        using callBackFunction = std::function<void()>;

        Timer(boost::asio::io_context &ioc, const callBackFunction &fn, boost::asio::chrono::seconds time):  timer(ioc), handler(fn),
                                                                                                          expire_time(time), cancle(false){}
        void Start(){
            timer.expires_after(expire_time);
            auto self(shared_from_this());

            timer.async_wait([self](boost::system::error_code ec){
                if(self->cancle)
                    return;
                self->handler();
                self->Start();
            });
        }

        void Reset(){
            timer.expires_after(expire_time);
        }

        void Cancle(){
            cancle = true;
        }

    private:
        bool cancle;
        boost::asio::steady_timer timer;
        callBackFunction handler;
        boost::asio::chrono::seconds expire_time;
    };
}

#endif //RAFT_TIMER_H
