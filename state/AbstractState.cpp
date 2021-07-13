//
// Created by 靖晏 on 2021/7/9.
//

#include "AbstractState.h"

namespace pod{
    bool AbstractState::UpdateTermAndLeader(int term, int leader) {
        if(term > context.GetTerm() || (term == context.GetTerm() && context.GetLeader()==0 && leader !=0 )){
            grape::Logger::INFO(format("Update term and leader new config: %1%, %2%, old config: %3%, %4%")
                                        %term%leader%context.GetTerm()%context.GetLeader());

            context.SetTerm(term);
            context.SetLeader(leader);
            return true;
        }
        return false;
    }
}