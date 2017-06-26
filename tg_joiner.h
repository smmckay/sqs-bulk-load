/*
 *    Copyright 2017 Steve McKay
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 */

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
