#include "testing.hpp"
#include "../sqlitemm.hh"

int main(void){
    Gio::init();
    // Creation of in-memory database should (almost) always succeed;
    auto a = Connection::create(":memory:");
    SHOULD("Newly created connection should be unique",
           a.unique());
    return 0;
}
