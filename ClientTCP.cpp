#include "ClientTCP.h"

#define DEBUG

ClientTCP::ClientTCP(const char *addr, const int port) : addr(addr), port(port) {

}

ClientTCP::~ClientTCP() {
    shouldDie = true;
    if (recvWorking) recvTh.detach();
    //if (recvWorking) recvTh.join();
#ifdef _WIN32
    shutdown(mainSocket, SD_BOTH);
    WSACleanup();
#elif __linux__
    shutdown(mainSocket, SHUT_RDWR);
#endif
}

int ClientTCP::init() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR)
        return LIL_ERROR;
#endif
    mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mainSocket == INVALID_SOCKET) {
        //printf("Error creating socket: %ld\n", WSAGetLastError());
#ifdef __WIN32
        WSACleanup();
#endif
        return LIL_ERROR;
    }

    sockaddr_in service{};
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(addr);
    service.sin_port = htons((u_short) port);
    if (connect(mainSocket, (sockaddr *) &service, sizeof(service)) == SOCKET_ERROR) {
        //printf( "Failed to connect.\n" );
#ifdef __WIN32
        WSACleanup();
#endif
        return LIL_ERROR;
    }
    startRecv();
#ifdef DEBUG
    printf("Init success\n");
#endif
    return LIL_SUCCESS;
}

int ClientTCP::send(const char *data, size_t size, unsigned int type) {
    int bytesSent;
    const size_t headerSize = sizeof(size_t) + sizeof(int) + sizeof(time_t);
    char *headerBuffer = new char[headerSize];
    time_t time = std::time(nullptr);
    headerBuffer[0] = static_cast<char>(size);
    headerBuffer[sizeof(size_t)] = static_cast<char>(type);
    headerBuffer[sizeof(size_t) + sizeof(unsigned int)] = static_cast<char>(time);

    bytesSent = ::send(mainSocket, headerBuffer, headerSize, 0);
    if (bytesSent <= 0) return SOCKET_ERROR;
    bytesSent = ::send(mainSocket, data, size, 0);
    delete[] headerBuffer;
#ifdef DEBUG
    printf("Sent data\n");
#endif
    return LIL_SUCCESS;
}

void ClientTCP::recvThread() {
    int bytesRecv = SOCKET_ERROR;
    const size_t headerSize = sizeof(size_t) + sizeof(int) + sizeof(time_t);
    char *headerBuffer = new char[headerSize];

    while (!shouldDie) {
        bytesRecv = recv(mainSocket, headerBuffer, headerSize, 0);
        if (shouldDie) break;
        if (bytesRecv > 0) {
            auto size = static_cast<size_t>(headerBuffer[0]);
            auto type = static_cast<unsigned int>(headerBuffer[sizeof(size_t)]);
            auto time = static_cast<time_t>(headerBuffer[sizeof(size_t) + sizeof(unsigned int)]);

            auto message = new Message(size, type, time);
            bytesRecv = recv(mainSocket, static_cast<char *>(message->data), size, 0);

            messagesMutex.lock();
            messages.push(message);
            messagesMutex.unlock();

#ifdef DEBUG
            printf("Received data: %s\n", static_cast<char *>(message->data));
#endif
        } else if (bytesRecv == 0) {
#ifdef DEBUG
            printf("Server quit\n");
#endif
            break;
        } else {
#ifdef DEBUG
            printf("Socket error\n");
#endif
            break;
        }
    }


}

int ClientTCP::startRecv() {
    recvWorking = true;
    recvTh = std::thread(&ClientTCP::recvThread, this);
    return LIL_SUCCESS;
}

Message *ClientTCP::popMessage() {
    messagesMutex.lock();
    auto rv = messages.back();
    messages.pop();
    messagesMutex.unlock();
    return rv;
}



