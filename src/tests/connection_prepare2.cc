#include <iostream>
#include <memory>

#include "../connection.hh"

int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));
    auto b = c->prepare("SELECT 1;");
    delete b;
    return 0;
}
