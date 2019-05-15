//
// Created by qaze on 15/05/2019.
//

#include "ServerTCP.h"

ServerTCP::ServerTCP(const char *addr, const int port) : addr(addr), port(port) {

}

ServerTCP::~ServerTCP() {
    if (recvWorking) recvTh.join();
    closesocket(mainSocket);
    WSACleanup();
}

int ServerTCP::init() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR)
        return LIL_ERROR;
    mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mainSocket == INVALID_SOCKET) {
        //printf("Error creating socket: %ld\n", WSAGetLastError());
        WSACleanup();
        return LIL_ERROR;
    }

    sockaddr_in service{};
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(addr);
    service.sin_port = htons((u_short) port);
    if (connect(mainSocket, (SOCKADDR *) &service, sizeof(service)) == SOCKET_ERROR) {
        //printf( "Failed to connect.\n" );
        WSACleanup();
        return LIL_ERROR;
    }
    return LIL_SUCCESS;
}