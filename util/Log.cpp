//
// Created by 靖晏 on 2021/7/8.
//

#include "Log.h"
#include <iostream>

using std::endl;

namespace grape{

    void Logger::INFO(const format &msg){
        std::cout<<"[INFO] "<<msg<<endl;
    }

    void Logger::WARN(const format &msg){
        std::cout<<"[WARN] "<<msg<<endl;
    }

    void Logger::ERROR(const format &message){
        std::cout<<"[ERROR] "<<message<<endl;
        exit(-1);
    }
}