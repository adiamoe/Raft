//
// Created by 靖晏 on 2021/7/8.
//

#include "Log.h"
#include <iostream>

using std::endl;

namespace grape{

    void Logger::INFO(const std::string &msg){
        std::cout<<"[INFO]"<<msg<<endl;
    }

    void Logger::WARN(const std::string &msg){
        std::cout<<"[WARN]"<<msg<<endl;
    }

    void Logger::ERROR(const std::string &message){
        std::cout<<"[ERROR]"<<message<<endl;
        exit(-1);
    }
}