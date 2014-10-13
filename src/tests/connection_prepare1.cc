#include <memory>

#include "../connection.hh"
#include "testing.hpp"

int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));

    SHOULD_THROW("preparation of empty query should fail",
                 SQLiteException,
                 { c->prepare(""); });

    return 0;
}
