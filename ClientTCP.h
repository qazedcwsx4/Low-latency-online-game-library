#ifndef LIL_LIBRARY_H
#define LIL_LIBRARY_H

#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
    #include <winsock2.h>
#elif __linux__
    #define SOCKET_ERROR (-1)
    #include <netinet/in.h>
    #include <libnet.h>
    #include <pcap.h>
#endif

#include <iostream>
#include <thread>
#include <queue>

void hello();

class ClientTCP {
public:
    const int LIL_ERROR = 1;
    const int LIL_SUCCESS = 0;
    std::queue <char*> messages;

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