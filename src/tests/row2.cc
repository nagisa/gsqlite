#include <memory>
#include <cmath>
#include <unistd.h>

#include "../connection.hh"
#include "../row.hh"
#include "testing.hpp"

// Tests variety of extract and operator[] function combinations.
int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));
    std::unique_ptr<Statement> s(c->prepare("SELECT 42, NULL, 3.1415926535;"));
    {
    auto future = s->next();
    future.wait();
    auto row = future.get();

    SHOULD("column count is 3",
           row->columns() == 3);
    SHOULD("first column is int equal to 42",
           row->extract<int64_t>(0) == 42);
    SHOULD("second column is close to pi",
           std::fabs(row->extract<double>(2) - 3.1415926535) < 1E-6L);

    SHOULD_THROW("extract with invalid type should fail",
                 SQLiteException,
                 { row->extract<const void *>(1); });

    SHOULD("get return nullptr on NULL",
           (*row)[1] == nullptr);
    SHOULD("get return non-nullptr on non-NULL",
           (*row)[2] != nullptr);
    }

    return 0;
}
