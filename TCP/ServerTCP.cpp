#include "ServerTCP.h"

#define DEBUG

ServerTCP::ServerTCP(const char *addr, const int port) : addr(addr), port(port) {

}

ServerTCP::~ServerTCP() {
    shouldDie = true;
    if (recvWorking) recvTh.detach();
#ifdef _WIN32
    shutdown(mainSocket, SD_BOTH);
    WSACleanup();
#elif __linux__
    shutdown(mainSocket, SHUT_RDWR);
#endif
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

int ServerTCP::recvThread() {
    fd_set socketReadSet;

    FD_ZERO(&socketMainSet);
    FD_ZERO(&socketReadSet);

    listen(mainSocket, 10);
    FD_SET(mainSocket, &socketMainSet);
    maxSocket = mainSocket;

    while (!shouldDie) {
        socketReadSet = socketMainSet;
        if (select(maxSocket + 1, &socketReadSet, nullptr, nullptr, nullptr) == SOCKET_ERROR) {
            return LIL_ERROR;
        }
        if (shouldDie) break;

        //process accept
        if (FD_ISSET(mainSocket, &socketReadSet)) {
            sockaddr_in clientAddr;
            SOCKET clientSocket;
            int addrSize = sizeof(clientAddr);;
            if ((clientSocket = accept(mainSocket, reinterpret_cast<sockaddr *>(&clientAddr), &addrSize)) ==
                SOCKET_ERROR) {
                return LIL_ERROR;
            }
#ifdef DEBUG
            printf("cs %d\n", clientSocket);
#endif
            if (clientSocket > maxSocket) {
                maxSocket = clientSocket;
            }
            FD_SET(clientSocket, &socketMainSet);
            //FD_SET(clientSocket, &socketReadSet);
            clientsList.emplace_back(clientSocket, clientAddr);

            simpleClientListMutex.lock();
            simpleClientList.emplace_back(clientsList.back());
            simpleClientListMutex.unlock();

#ifdef DEBUG
            printf("New connection\n");
#endif
        }

        //process the rest
        for (auto i = clientsList.begin(); !clientsList.empty() && i != clientsList.end(); ++i) {
            int bytesRecv = SOCKET_ERROR;

            if (FD_ISSET(i->socket, &socketReadSet)) {
                char headerBuffer[Message::headerSize];

                bytesRecv = recv(i->socket, headerBuffer, Message::headerSize, 0);
                //printf("Error creating socket: %ld\n", WSAGetLastError());

                if (bytesRecv > 0) {

                    auto message = new Message(headerBuffer);
                    bytesRecv = recv(i->socket, static_cast<char *>(message->data), message->size, 0);

                    i->messagesMutex.lock();
                    i->messages.push(message);
                    i->messagesMutex.unlock();

#ifdef DEBUG
                    printf("Received data: %s\n", static_cast<char *>(message->data));
#endif
                } else if (bytesRecv == 0) {
                    //handle disconnect

                    FD_CLR(i->socket, &socketMainSet);

                    //delete entry from simpleClientList
                    simpleClientListMutex.lock();
                    auto itr = simpleClientList.begin();
                    while (itr != simpleClientList.end()){
                        if (itr->socket == i->socket)
                        {
                            simpleClientList.erase(itr);
                            break;
                        }
                    }
                    simpleClientListMutex.unlock();

                    closesocket(i->socket);
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

int ServerTCP::startRecv() {
    recvWorking = true;
    recvTh = std::thread(&ServerTCP::recvThread, this);
    return LIL_SUCCESS;
}

const std::list<SimpleClientData> &ServerTCP::getSimpleClientList() const {
    return simpleClientList;
}