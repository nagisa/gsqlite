#include <memory>
#include <unistd.h>

#include "../connection.hh"
#include "../row.hh"
#include "testing.hpp"

int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));
    std::unique_ptr<Statement> s(c->prepare("SELECT 42;"));
    {
    auto future = s->next();
    future.wait();
    SHOULD("future is valid after wait",
           future.valid());
    auto row = future.get();
    SHOULD("row should have 1 column",
           row->columns() == 1);

    future = s->next();
    future.wait();
    SHOULD_THROW("row should be invalid after next() on statement",
                 std::logic_error,
                 { row->columns(); });
    return 0;
    }
}
