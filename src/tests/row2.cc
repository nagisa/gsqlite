#include <iostream>
#include <memory>
#include <cmath>
#include <unistd.h>

#include "../connection.hpp"
#include "../row.hpp"

// Tests variety of extract and operator[] function combinations.
int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));
    std::unique_ptr<Statement> s(c->prepare("SELECT 42, NULL, 3.1415926535;"));
    {
    auto future = s->next();
    future.wait();
    auto row = future.get();

    std::cout << row->show() << std::endl;
    if(row->columns() != 3){
        std::cerr << "column count != 3" << std::endl;
        return 1;
    }

    if(row->extract<int64_t>(0) != 42) {
        std::cerr << "extract gives incorrect results" << std::endl;
        return 1;
    }

    if(std::fabs(row->extract<double>(2) - 3.1415926535) > 1E-6L) {
        std::cerr << "extract gives incorrect results" << std::endl;
        return 1;
    }

    try {
        row->extract<const void *>(1);
        std::cerr << "extract does not throw on NULL" << std::endl;
        return 1;
    } catch (SQLiteException& e) { }

    if((*row)[1] != nullptr) {
        std::cerr << "get does not return nullptr" << std::endl;
        return 1;
    }

    if((*row)[2] == nullptr) {
        std::cerr << "get fails on non-null" << std::endl;
        return 1;
    }
    }

    return 0;
}
