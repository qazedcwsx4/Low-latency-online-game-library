#ifndef LIL_LIBRARY_H
#define LIL_LIBRARY_H

#include <cstdio>
#include <cstdlib>

#include <winsock2.h>
#include <iostream>
#include <thread>

void hello();

class ClientTCP {
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
    ClientTCP(const char *addr, const int port);

    ~ClientTCP();

    int init();

    int send(const char *message);


private:
    void recvThread();
    int startRecv();

};

#endif