#include "ServerTCP.h"

#define DEBUG

ServerTCP::ServerTCP(const char *addr, const int port) : addr(addr), port(port) {

}

ServerTCP::~ServerTCP() {
    closesocket(mainSocket);
    WSACleanup();
}

int ServerTCP::init() {
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
    if (bind(mainSocket, (sockaddr *) &service, sizeof(service)) == SOCKET_ERROR) {
        //printf( "Failed to connect.\n" );
#ifdef __WIN32
        WSACleanup();
#endif
        return LIL_ERROR;
    }


#ifdef DEBUG
    printf("Init success\n");
#endif
    return LIL_SUCCESS;
}

int ServerTCP::start() {
    fd_set socketReadSet;

    FD_ZERO(&socketMainSet);
    FD_ZERO(&socketReadSet);

    listen(mainSocket, 10);
    FD_SET(mainSocket, &socketMainSet);
    maxSocket = mainSocket;

    while (true) {
        socketReadSet = socketMainSet;
        if (select(maxSocket + 1, &socketReadSet, nullptr, nullptr, nullptr) == SOCKET_ERROR) {
            return LIL_ERROR;
        }

        //process accept
        if (FD_ISSET(mainSocket, &socketReadSet)) {
            sockaddr_in clientAddr;
            SOCKET clientSocket;
            int addrSize = sizeof(clientAddr);;
            if ((clientSocket = accept(mainSocket, reinterpret_cast<sockaddr *>(&clientAddr), &addrSize)) ==
                SOCKET_ERROR) {
                return LIL_ERROR;
            }
            printf("cs %d\n", clientSocket);

            if (clientSocket > maxSocket) {
                maxSocket = clientSocket;
            }
            FD_SET(clientSocket, &socketMainSet);
            //FD_SET(clientSocket, &socketReadSet);
            clientsList.emplace_back(clientSocket, clientAddr);

#ifdef DEBUG
            printf("New connection\n");
#endif
        }

        //process the rest
        for (auto i = clientsList.begin(); !clientsList.empty() && i != clientsList.end(); ++i) {
            int bytesRecv = SOCKET_ERROR;

            if (FD_ISSET(i->socket, &socketReadSet)) {
                const size_t headerSize = sizeof(size_t) + sizeof(int) + sizeof(time_t);
                char *headerBuffer = new char[headerSize];

                bytesRecv = recv(i->socket, headerBuffer, headerSize, 0);

                if (bytesRecv > 0) {
                    auto size = static_cast<size_t>(headerBuffer[0]);
                    auto type = static_cast<unsigned int>(headerBuffer[sizeof(size_t)]);
                    auto time = static_cast<time_t>(headerBuffer[sizeof(size_t) + sizeof(unsigned int)]);

                    auto message = new Message(size, type, time);
                    bytesRecv = recv(i->socket, static_cast<char *>(message->data), size, 0);

                    i->messagesMutex.lock();
                    i->messages.push(message);
                    i->messagesMutex.unlock();

#ifdef DEBUG
                    printf("Received data: %s\n", static_cast<char *>(message->data));
#endif
                } else if (bytesRecv == 0) {
                    //handle disconnect

                    closesocket(i->socket);
                    FD_CLR(i->socket, &socketMainSet);
                    i = clientsList.erase(i);
#ifdef DEBUG
                    printf("Client disconnected\n");
#endif
                } else {
#ifdef DEBUG
                    printf("Error creating socket: %ld\n", WSAGetLastError());
#endif
                }
            }
        }
    }
}
