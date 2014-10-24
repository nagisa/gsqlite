#include <giomm.h>

#include "../connection.hh"
#include "testing.hpp"

int main(void){
    Gio::init();
    auto c = Connection::create(":memory");

    SHOULD_THROW("preparation of empty query should fail",
                 SQLiteException,
                 { c->prepare(""); });

    return 0;
}
