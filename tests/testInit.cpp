#include "../lib/TCP/ClientTCP.h"
#include "../lib/TCP/ServerTCP.h"

int main() {
    ServerTCP serverTCP("127.0.0.1", 4201);
    ClientTCP clientTCP("127.0.0.1", 4201);

    if (serverTCP.init() < 0) return 1;
    if (clientTCP.init() < 0) return 2;

    Sleep(100);

    if (serverTCP.getSimpleClientList().size() != 1) return 3;

    return 0;
}