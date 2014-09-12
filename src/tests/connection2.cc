#include <iostream>

#include "../connection.hpp"

int main(void){
    // Creation of database on directory should fail
    try {
        Connection a("/");
    } catch (SQLiteException& e){
        return 0;
    }
    std::cerr << "Opening database for directory did not fail" << std::endl;
    return 1;
}
