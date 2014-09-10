#include <iostream>

#include "../connection.hpp"

int main(void){
    // Creation of database on directory should fail
    try {
        Connection a("/");
    } catch (SQLiteException& e){
        std::cerr << e.what() << std::endl;
        return 0;
    }
    return 1;
}
