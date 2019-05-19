//
// Created by qaze on 18/05/2019.
//

#include "Message.h"

Message::Message(size_t size, unsigned int type, time_t time) : size(size), type(type), time(time) {
    data = std::malloc(size);
}

Message::~Message() {
    free(data);
}
