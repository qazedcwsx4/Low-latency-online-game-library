//
// Created by qaze on 15/05/2019.
//

#ifndef LIL_SERVERTCP_H
#define LIL_SERVERTCP_H

#include <cstdio>
#include <cstdlib>
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <queue>

class ServerTCP {
public:
    const int LIL_ERROR = 1;
    const int LIL_SUCCESS = 0;

private:
    const char *addr;
    const int port;
    bool shouldDie = false;
    bool recvWorking = false;
    SOCKET mainSocket;
    std::thread recvTh;

public:
    ServerTCP(const char *addr, const int port);

    ~ServerTCP();

    int init();

private:

};


#endif //LIL_SERVERTCP_H
