#include "lib/TCP/ClientTCP.h"
#include "lib/TCP/ServerTCP.h"


int main() {
    ServerTCP succ("127.0.0.1", 4202);
    std::cout << "Init: " << succ.init();

    const std::list<SimpleClientData> &scd = succ.getSimpleClientList();

    while (true) {
        Sleep(1000);
        printf("clients (%d):\n", scd.size());
        if (succ.sendAll("tes2\n", 6, 0) == SOCKET_ERROR) break;
        if (!scd.empty()) {
            succ.send(scd.front(), "tes3\n", 6, 0);
        }
        for (auto i = scd.begin(); i != scd.end(); ++i) {
            printf("%d %s\n", i->socket, inet_ntoa(i->ip));
        }
    }
    return 0;
}