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
        return 1;
    }
    auto row = future.get();

    if(row->columns() != 1){
        return 1;
    }

    future = s->next();
    future.wait();

    try{
        row->columns();
        return 1;
    } catch (std::logic_error& e) {}
    }
    return 0;
}
