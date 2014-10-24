#include <giomm.h>

#include "testing.hpp"
#include "../connection.hh"

// Test that memory in connections and statements are managed correctly.
int main(void){
    Gio::init();
    auto c = Connection::create(":memory:");
    auto uc = c.use_count();
    auto b = c->prepare("SELECT 1;");
    SHOULD("connection use count after a statement is prepared should be "
           "increased",
           c.use_count() == uc + 1);
    delete b;
    SHOULD("connection use count after a statement is destroyed should be "
           "decreased",
           c.use_count() == uc);
    return 0;
}
