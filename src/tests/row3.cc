#include <memory>
#include <algorithm>
#include <giomm.h>

#include "../connection.hh"
#include "../row.hh"
#include "testing.hpp"

// It should be possible to have multiple statements active at the same time.
int main(void){
    Gio::init();
    auto c = Connection::create(":memory:");

    std::vector<std::unique_ptr<Statement>> s(3);
    s[0].reset(c->prepare("SELECT 42, NULL, 3.1415926535;"));
    s[1].reset(c->prepare("SELECT 10, \"Hello\", 2.7182818284;"));
    s[2].reset(c->prepare("SELECT 0, NULL, 0.0;"));

    {
    std::vector<Statement::result_t> futures;

    int index = 0;
    int tested = 0;
    for(auto st = s.begin(); st != s.end(); st++, index++) {
        auto row = (*st)->next();
        row->monitor([row, index, &tested](Row::state_t s){
            SHOULD("row is RESOLVED",
                   s == Row::RESOLVED)

            // Should not throw
            row->extract<int64_t>(0);
            row->extract<double>(2);

            if(index == 1)
                SHOULD("second column in second statement is non-null",
                       row->operator[](1) != nullptr)
            else
                SHOULD("second column in non-second statement are null",
                       row->operator[](1) == nullptr)
            tested++;
            return false;
        });
        futures.push_back(row);
    }

    auto mainloop = Glib::MainLoop::create(false);
    auto ctx = mainloop->get_context();

    while(tested != 3){
        ctx->iteration(false);
    }
    while(ctx->pending()) ctx->iteration(false);
    }

    return 0;
}
