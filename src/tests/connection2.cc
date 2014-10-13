#include "../connection.hh"
#include "testing.hpp"

int main(void){
    SHOULD_THROW("creation of database on directory should fail",
                 SQLiteException,
                 { Connection a("/"); });
    return 0;
}
