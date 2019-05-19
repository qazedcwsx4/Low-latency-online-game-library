//
// Created by qaze on 18/05/2019.
//

#ifndef LIL_MESSAGE_H
#define LIL_MESSAGE_H

#include <stdlib.h>

class Message {
public:
    int size;
    unsigned int type;
    time_t time;
    void *data;
public:
    Message(size_t size, unsigned int type, time_t time);

    ~Message();
};


#endif //LIL_MESSAGE_H
