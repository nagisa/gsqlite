#include <iostream>
#include <memory>
#include <algorithm>
#include <unistd.h>

#include "../connection.hpp"
#include "../row.hpp"

// It should be possible to have multiple statements active at the same time.
int main(void){
    std::unique_ptr<Connection> c(new Connection(":memory:"));

    std::vector<std::unique_ptr<Statement>> s(3);
    s[0].reset(c->prepare("SELECT 42, NULL, 3.1415926535;"));
    s[1].reset(c->prepare("SELECT 10, \"Hello\", 2.7182818284;"));
    s[2].reset(c->prepare("SELECT 0, NULL, 0.0;"));

    {
    std::vector<std::future<std::shared_ptr<Row>>> futures;

    for(auto st = s.begin(); st != s.end(); st++) {
        futures.push_back(std::move((*st)->next()));
    }

    auto index = 0;
    for(auto el = futures.begin(); el != futures.end(); el++, index++){
        el->wait();
        auto row = el->get();
        row->extract<int64_t>(0);
        row->extract<double>(2);
        if((index != 1 && (*row)[1] != nullptr) ||
           (index == 1  && (*row)[1] == nullptr)){
            return 1;
        }
    }
    }

    return 0;
}
