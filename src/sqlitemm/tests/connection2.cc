#include "testing.hpp"
#include "../sqlitemm.hh"

int main(void){
    Gio::init();
    SHOULD_THROW("creation of database on directory should fail",
                 SQLiteError,
                 { Connection::create("/"); });
    return 0;
}
