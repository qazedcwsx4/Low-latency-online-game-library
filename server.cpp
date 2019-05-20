#include "TCP/ClientTCP.h"
#include "TCP/ServerTCP.h"

int main() {
    ServerTCP succ("127.0.0.1", 4202);
    std::cout << "Init: " << succ.init();
    succ.start();
    return 0;
}