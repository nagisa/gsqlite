#include <iostream>
#include <memory>
#include <unistd.h>

#include "../connection.hpp"
#include "../row.hpp"

int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));
    std::unique_ptr<Statement> s(c->prepare("SELECT 42;"));
    {
    auto future = s->next();
    future.wait();
    if(!future.valid()){
        std::cerr << "future invalid before retrieval" << std::endl;
        return 1;
    }
    auto row = future.get();

    std::cout << row->show() << std::endl;
    if(row->columns() != 1){
        std::cerr << "column count != 1" << std::endl;
        return 1;
    }
    if(row->extract<int64_t>(0) != 42) {
        std::cerr << "extract gives incorrect results" << std::endl;
        return 1;
    }
    }
    return 0;
}
