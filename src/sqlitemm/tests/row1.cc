#include <memory>

#include "testing.hpp"
#include "../sqlitemm.hh"

int main(void){
    Gio::init();
    auto c = Connection::create(":memory:");
    auto s = std::shared_ptr<Statement>(c->prepare("SELECT 42;"));
    auto loop = Glib::MainLoop::create();
    {
    s->next_async([s](Glib::RefPtr<Gio::AsyncResult>& res){
        auto r = s->next_finish(res);
        SHOULD("row should have 1 column",
               r->columns() == 1);
        delete r;
    });

    s->next_async([s, loop](Glib::RefPtr<Gio::AsyncResult>& res){
        auto r = s->next_finish(res);
        SHOULD("row should be nullptr when there’s no more rows",
               r == nullptr);
        delete r;
        loop->quit();
    });
    }

    loop->run();

    return 0;
}
