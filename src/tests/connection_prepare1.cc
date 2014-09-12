#include <iostream>
#include <memory>

#include "../connection.hpp"

int main(void){
    std::unique_ptr<Connection> c;

    try {
        c = std::unique_ptr<Connection>(new Connection(":memory:"));
    } catch (SQLiteException& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        c->prepare("");
    } catch (SQLiteException& e){
        return 0;
    }

    std::cerr << "Preparation of empty query did not fail" << std::endl;
    return 1;
}
