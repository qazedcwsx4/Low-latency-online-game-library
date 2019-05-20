#include "TCP/ClientTCP.h"
#include "TCP/ServerTCP.h"

int main() {
    ClientTCP succ("127.0.0.1", 4202);
    std::cout << "Init: " << succ.init();
    while (true) {
        if (succ.send("test\n", 6, 0) == SOCKET_ERROR) break;
        Sleep(1000);
    }


    return 0;
}