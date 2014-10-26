#include <memory>
#include <cmath>

#include "testing.hpp"
#include "../sqlitemm.hh"

// Tests variety of extract and operator[] function combinations.
int main(void){
    Gio::init();
    auto c = Connection::create(":memory:");
    std::shared_ptr<Statement> s(c->prepare("SELECT 42, NULL, 3.1415926535;"));
    auto loop = Glib::MainLoop::create();
    {
    s->next_async([s, loop](Glib::RefPtr<Gio::AsyncResult>& res){
        auto row = s->next_finish(res);
        SHOULD("column count is 3",
               row->columns() == 3);
        SHOULD("first column is int equal to 42",
               row->extract<int64_t>(0) == 42);
        SHOULD("second column is close to pi",
               std::fabs(row->extract<double>(2) - 3.1415926535) < 1E-6L);

        SHOULD_THROW("extract with invalid type should fail",
                     SQLiteError,
                     { row->extract<const void *>(1); });

        SHOULD("get return nullptr on NULL",
               (*row)[1] == nullptr);
        SHOULD("get return non-nullptr on non-NULL",
               (*row)[2] != nullptr);
        loop->quit();
        delete row;
    });
    }

    loop->run();

    return 0;
}
