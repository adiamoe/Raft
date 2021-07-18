//
// Created by 靖晏 on 2021/7/9.
//

#ifndef RAFT_LOGSTORE_H
#define RAFT_LOGSTORE_H

#include <vector>
#include <string>

namespace pod{
    class LogStore{
    private:
        struct LogEntry{
            int term;
            std::string log;
        };

        std::vector<LogEntry> entry;
        int lastIndex;
    public:

        LogStore():lastIndex(0){
            entry.push_back({0, ""});
        }

        LogStore(LogStore &store) = default;

        int LastIndex() const {
            return lastIndex;
        }

        int Term(int index) const{
            return entry[index].term;
        }

        size_t entrySize() const{
            return entry.size();
        }

        void append(int term, int index, const std::string &log){
            if(index == entry.size()){
                entry.push_back({term, log});
            }
            else{
                entry[index] = {term, log};
            }
            lastIndex = index;
        }

        void append(int term, const std::string &log){
            entry.push_back({term, log});
            lastIndex = entrySize() - 1;
        }
    };
}


#endif //RAFT_LOGSTORE_H
