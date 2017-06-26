//
// Created by McKay, Steven M. (PLUM) on 6/26/17.
//

#ifndef SQS_BULK_LOAD_TG_JOINER_H
#define SQS_BULK_LOAD_TG_JOINER_H

#include <boost/thread/thread.hpp>

class tg_joiner {
public:
    tg_joiner(boost::thread_group &tg) : _tg(tg) {}
    ~tg_joiner() {
        _tg.join_all();
    }
private:
    boost::thread_group &_tg;
};

#endif //SQS_BULK_LOAD_TG_JOINER_H
