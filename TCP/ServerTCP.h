#ifndef LIL_SERVERTCP_H
#define LIL_SERVERTCP_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <queue>
#include <mutex>
#include <list>

#ifdef _WIN32
#include <winsock2.h>
#elif __linux__
#define SOCKET_ERROR (-1)
#include <netinet/in.h>
#include <libnet.h>
#include <pcap.h>
#endif

#include "../Message.h"

struct ClientData {
    SOCKET socket;
    sockaddr_in data;
    std::queue<Message *> messages;
    std::mutex messagesMutex;

    ClientData(SOCKET socket, sockaddr_in data) : socket(socket), data(data) {

    }
};

class ServerTCP {
public:
    const int LIL_ERROR = 1;
    const int LIL_SUCCESS = 0;

private:
    const char *addr;
    const int port;
    SOCKET mainSocket;
    fd_set socketMainSet;
    int maxSocket;
    std::list<ClientData> clientsList;

public:
    ServerTCP(const char *addr, const int port);

    ~ServerTCP();

    int init();

    int start();

private:

};


#endif
