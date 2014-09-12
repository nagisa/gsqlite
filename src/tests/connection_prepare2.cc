#include <iostream>
#include <memory>

#include "../connection.hpp"

int main(void){
    std::unique_ptr<Connection> c;

    try {
        // UGLY!
        c = std::unique_ptr<Connection>(new Connection(":memory:"));
    } catch (SQLiteException& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        auto b = c->prepare("SELECT 1;");
        delete b;
    } catch (SQLiteException& e){
        std::cerr << "Preparation of valid query did fail" << std::endl;
        return 1;
    }

    return 0;
}
