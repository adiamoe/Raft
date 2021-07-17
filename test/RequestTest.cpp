//
// Created by 靖晏 on 2021/7/11.
//

#include "../storage/Request.h"
#include <iostream>

using namespace pod;

int main(){
    AppendRequest re;
    re.type = (REQUEST_TYPE) 1;
    re.leaderID = 1;
    re.leaderTerm = 2;
    re.commitIndex = 3;
    re.lastEntryTerm = 4;
    re.lastEntryIndex = 5;
    re.entryIndex = 6;
    re.heartBeat = false;
    re.log = "Hello World";

    std::string json_to = CreateAppendRequest(re);
    std::cout<<json_to<<std::endl;

    json js = json::parse(json_to);
    AppendRequest re2;
    re2 = GetAppendRequest(js);
    std::cout<<re2.log<<std::endl;
    std::cout<<(re2.type == APPEND)<<std::endl;

    return 0;
}