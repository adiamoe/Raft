//
// Created by 靖晏 on 2021/7/9.
//

#include "ServerContext.h"

namespace pod{
    AppendRequest ServerContext::Append(string &log) {
        store.append(term, log);
        AppendRequest re;
        re.log = log;
        re.entryIndex = store.LastIndex();
        re.lastEntryIndex = re.entryIndex - 1;
        re.heartBeat = false;
        re.leaderTerm = term;
        re.leaderID = leader;
        re.commitIndex = commitIndex;
        re.lastEntryTerm = store.Term(re.lastEntryIndex);
        return re;
    }

    VoteRequest ServerContext::Vote(){
        VoteRequest re;
        re.lastEntryIndex = store.LastIndex();
        re.lastEntryTerm = store.Term(re.lastEntryIndex);
        re.candidateTerm = term;
        re.candidateID = serverId;
        return re;
    }
}
