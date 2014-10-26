#include <memory>
#include <algorithm>

#include "testing.hpp"
#include "../sqlitemm.hh"

// It should be possible to have multiple statements active at the same time.
int main(void){
    Gio::init();
    auto c = Connection::create(":memory:");

    std::vector<std::shared_ptr<Statement>> s(3);
    s[0].reset(c->prepare("SELECT 42, NULL, 3.1415926535;"));
    s[1].reset(c->prepare("SELECT 10, \"Hello\", 2.7182818284;"));
    s[2].reset(c->prepare("SELECT 0, NULL, 0.0;"));

    int tested = 0;
    int index = 0;
    {
    for(auto stmt = s.begin(); stmt < s.end(); stmt += 1, index += 1){
        (*stmt)->next_async([stmt, &tested, index](Glib::RefPtr<Gio::AsyncResult>& res){
            auto row = (*stmt)->next_finish(res);
            SHOULD("row should not be null", row != nullptr);

            // TODO: fail if throws
            row->extract<int64_t>(0);
            row->extract<double>(2);

            if(index == 1)
                SHOULD("second column in second statement is non-null",
                       row->operator[](1) != nullptr)
            else
                SHOULD("second column in non-second statement are null",
                       row->operator[](1) == nullptr)

            tested += 1;
            delete row;
        });
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
