//
// Created by 靖晏 on 2021/7/12.
//

#include "../util/Log.h"

int main()
{
    grape::Logger::INFO(format("This is %1% message for %2% format")%1%"boost");

    return 0;
}

