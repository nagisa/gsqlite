#include <iostream>

#include "../connection.hh"

int main(void){
    // Creation of in-memory database should (almost) always succeed;
    Connection a(":memory:");
    return 0;
}
