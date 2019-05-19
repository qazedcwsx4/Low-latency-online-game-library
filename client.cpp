#include "ClientTCP.h"
#include "ServerTCP.h"

int main() {
    ClientTCP succ("127.0.0.1", 4202);
    std::cout << "Init: " << succ.init();
    while (true){
        succ.send("test\n",6,0);
        Sleep(1000);
    }


    return 0;
}