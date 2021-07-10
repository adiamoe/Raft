//
// Created by 靖晏 on 2021/7/10.
//

#include "../util/timer.h"
#include <iostream>

void print(){
    std::cout<<"Timer expired!"<<std::endl;
}

int main(){

    boost::asio::io_context io_;

    //注意要用shared_ptr
    auto timer = std::make_shared<grape::Timer>(io_, &print, boost::asio::chrono::seconds(1));
    timer->Start();

    io_.run();

    int i=0;
    for(; i<500; ++i)
        timer->reset();

    std::cout<<i;
    return 0;
}