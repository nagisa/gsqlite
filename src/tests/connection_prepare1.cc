#include <iostream>
#include <memory>

#include "../connection.hpp"

int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));

    try {
        c->prepare("");
    } catch (SQLiteException& e){
        return 0;
    }

    std::cerr << "Preparation of empty query did not fail" << std::endl;
    return 1;
}
