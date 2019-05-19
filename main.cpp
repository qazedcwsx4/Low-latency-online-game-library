//
// Created by qaze on 14/05/2019.
//
#include "ClientTCP.h"

int main() {
    //hello();
    ClientTCP succ("127.0.0.1", 4200);
    std::cout << "XD: " << succ.init();
    while (true){
        succ.send("test\n",6,0);
    Sleep(1000);
    }
    return 0;
}