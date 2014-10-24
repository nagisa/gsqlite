#include <memory>
#include <giomm.h>

#include "../connection.hh"
#include "../row.hh"
#include "testing.hpp"

int main(void){
    Gio::init();
    auto c = Connection::create(":memory:");
    auto s = std::unique_ptr<Statement>(c->prepare("SELECT 42;"));
    auto loop = Glib::MainLoop::create();
    {
    auto row = s->next();
    SHOULD("row just created should be PRISTINE (a bit racy)",
           row->state() == Row::PRISTINE);
    row->monitor([row](Row::state_t s){
        SHOULD("row should be RESOLVED after wait",
               s == Row::RESOLVED);
        SHOULD("callback argument should match with property",
               s == row->state().get_value());
        SHOULD("row should have 1 column",
               row->columns() == 1);
        return false;
    });


    row = s->next();
    row->monitor([row, loop](Row::state_t s){
        SHOULD("row should be DONE when there’s no more rows",
               s == Row::DONE);
        SHOULD_THROW("row should be unusable when not RESOLVED",
                     std::logic_error,
                     { row->columns(); });
        loop->quit();
        return false;
    });
    }

    loop->run();

    return 0;
}
