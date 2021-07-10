//
// Created by 靖晏 on 2021/7/9.
//

#ifndef RAFT_LOGSTORE_H
#define RAFT_LOGSTORE_H

#include <vector>

namespace pod{
    class LogStore{
    private:
        std::vector<long> entry;
        int lastIndex;
    public:

        int LastIndex() {
            return lastIndex;
        }

        void append(int index, long log){
            if(index > entry.size()-1)
                entry.push_back(log);
            else
                entry[index] = log;

            lastIndex = index;
        }
    };
}


#endif //RAFT_LOGSTORE_H
