//
// Created by qaze on 18/05/2019.
//

#include <algorithm>
#include "Message.h"

Message::Message(size_t size, int type, time_t time) : size(size), type(type), timeSent(time) {
    data = std::malloc(size);
}

Message::Message(char *headerBuffer) {
    parseHeader(headerBuffer, size, type, timeSent, originalSender);
    data = std::malloc(size);
}

Message::~Message() {
    free(data);
}

void Message::createHeader(char *headerBuffer, size_t size, int type, long timeSent,
                           int originalSender) {
    std::copy(reinterpret_cast<char *>(&size),
              reinterpret_cast<char *>(&size) + sizeof(size_t),
              headerBuffer);

    std::copy(reinterpret_cast<char *>(&type),
              reinterpret_cast<char *>(&type) + sizeof(int),
              headerBuffer + sizeof(size_t));

    std::copy(reinterpret_cast<char *>(&timeSent),
              reinterpret_cast<char *>(&timeSent) + sizeof(long),
              headerBuffer + sizeof(size_t) + sizeof(int));

    std::copy(reinterpret_cast<char *>(&originalSender),
              reinterpret_cast<char *>(&originalSender) + sizeof(int),
              headerBuffer + sizeof(size_t) + sizeof(int) + sizeof(long));
}

void Message::parseHeader(char *headerBuffer, size_t &size, int &type, long &timeSent,
                          int &originalSender) {

    size = *reinterpret_cast<size_t *>(headerBuffer);
    type = *reinterpret_cast<int *>(headerBuffer + sizeof(size_t));
    timeSent = *reinterpret_cast<time_t *>(headerBuffer + sizeof(size_t) + sizeof(int));
    originalSender = *reinterpret_cast<int *>(headerBuffer + sizeof(size_t) + sizeof(long) + sizeof(int));
}
