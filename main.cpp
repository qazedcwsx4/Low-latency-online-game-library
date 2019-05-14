//
// Created by qaze on 14/05/2019.
//
#include "library.h"

int main() {
    //hello();
    ClientTCP succ("127.0.0.1", 4200);
    succ.init();
    succ.send("pedal");
    return 0;
}