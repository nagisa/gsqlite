#include "testing.hpp"
#include "../sqlitemm.hh"

int main(void){
    Gio::init();
    auto c = Connection::create(":memory");

    SHOULD_THROW("preparation of empty query should fail",
                 SQLiteError,
                 { c->prepare(""); });

    return 0;
}
