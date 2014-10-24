#include <giomm.h>

#include "testing.hpp"
#include "../connection.hh"

int main(void){
    Gio::init();
    SHOULD_THROW("creation of database on directory should fail",
                 SQLiteException,
                 { Connection::create("/"); });
    return 0;
}
