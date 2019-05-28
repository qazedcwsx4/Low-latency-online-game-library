#include "../lib/TCP/ClientTCP.h"
#include "../lib/TCP/ServerTCP.h"

int main() {
    Sleep(100);

    ServerTCP serverTCP("127.0.0.1", 4201);
    ClientTCP clientTCP("127.0.0.1", 4201);

    if (serverTCP.init() != 0) return -1;
    if (clientTCP.init() != 0) return -1;

    Sleep(100);

    if (clientTCP.send("TEST", 5, 0) != 0) return 1;

    Sleep(100);

    std::list<SimpleClientData> scdl = serverTCP.getSimpleClientList();
    SimpleClientData &scd = scdl.front();
    auto message = serverTCP.getMessage(scd);
    if (message == nullptr) return 2;

    if (message->type != 0 || message->size != 5 || strcmp(static_cast<char *>(message->data), "TEST") != 0) return 3;

    return 0;
}