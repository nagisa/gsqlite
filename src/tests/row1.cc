#include <iostream>
#include <memory>
#include <unistd.h>

#include "../connection.hpp"
#include "../row.hpp"

int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));
    auto b = c->prepare("SELECT 1;");
    auto future = b->next();
    std::cout << future.valid() << std::endl;
    future.wait();
    std::cout << future.valid() << std::endl;
    auto row = future.get();
    std::cout << row.show() << std::endl;
    delete b;
    return 0;
}
