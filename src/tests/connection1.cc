#include <iostream>

#include "../connection.hpp"

int main(void){
    // Creation of in-memory database should (almost) always succeed;
    try {
        Connection a(":memory:");
    } catch (SQLiteException& e){
        std::cerr << "Error was thrown for :memory:\n";
        return 1;
    }
    return 0;
}
