#include "ClientTCP.h"


void hello() {
    std::cout << "Hello, Worldd!" << std::endl;
}

ClientTCP::ClientTCP(const char *addr, const int port) : addr(addr), port(port) {

}

ClientTCP::~ClientTCP() {
    if (recvWorking) recvTh.join();
    closesocket(mainSocket);
    WSACleanup();
}

int ClientTCP::init() {
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
    startRecv();
    return LIL_SUCCESS;
}

int ClientTCP::send(const char *message) {
    int bytesSent;
    bytesSent = ::send(mainSocket, message, strlen(message), 0);
    return LIL_SUCCESS;
}

void ClientTCP::recvThread() {
    char recvbuf[32] = "";
    int bytesRecv = SOCKET_ERROR;

    while (!shouldDie) {
        for (int i = 0; i < 32; ++i) {
            recvbuf[i] = ' ';
        }
        bytesRecv = recv(mainSocket, recvbuf, 32, 0);
        printf("Received text: %s\n", recvbuf);
    }
}

int ClientTCP::startRecv() {
    recvWorking = true;
    recvTh = std::thread(&ClientTCP::recvThread, this);
}



