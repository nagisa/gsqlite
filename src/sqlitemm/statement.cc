#include <vector>
#include <utility>
#include <gio/gio.h>

#include "error.hh"
#include "statement.hh"
#include "row.hh"
#include "value.hpp"

Statement::Statement(sqlite3_stmt *stmt, connection_ptr_t c, schedule_fn_t sch)
    : Glib::ObjectBase(typeid(Statement))
    , statement(stmt)
    , schedule_fn(sch)
    , connection(c)
{
    if(stmt == NULL)
        throw SQLiteError(_SQLITE_NULL_STATEMENT);
}

Statement::Statement(Statement&& o)
    : statement(std::move(o.statement))
    , schedule_fn(std::move(o.schedule_fn))
{
    o.statement = nullptr;
}

Statement::~Statement()
{
    sqlite3_finalize(this->statement);
}

int
Statement::columns()
{
    return sqlite3_column_count(this->statement);
}

const char *
Statement::origin_name(const int n)
{
    return sqlite3_column_origin_name(this->statement, n);
}

int
Statement::column_type(const int n)
{
    return sqlite3_column_type(this->statement, n);
}

const char*
Statement::column_decltype(const int n)
{
    return sqlite3_column_decltype(this->statement, n);
}

// ---------------------------------------------------------------------------
void
get_next_row(Statement *stmt, GTask *task){
    int ret = SQLITE_BUSY;
    while(true){
        if(g_task_return_error_if_cancelled(task))
            return;
        if((ret = sqlite3_step(stmt->statement)) != SQLITE_BUSY)
            break;
    };
    try {
    switch(ret) {
        case SQLITE_DONE:
            g_task_return_pointer(task, nullptr, nullptr);
            break;
        case SQLITE_ROW:
            {
            std::vector<std::unique_ptr<Value>> vals(stmt->columns());
            int c = 0;
            for(auto i = vals.begin(); i < vals.end(); i += 1, c += 1){
                *i = std::unique_ptr<Value>(
                    mkValue(sqlite3_column_value(stmt->statement, c))
                );
            }
            g_task_return_pointer(task, new Row(std::move(vals)),
                                  [](gpointer data){
                delete static_cast<Row *>(data);
            });
            }
            break;
        default:
            throw SQLiteError(*stmt->connection);
            break;
    }
    } catch (Glib::Error &e) {
        g_task_return_error(task, g_error_copy(e.gobj()));
    } catch (std::exception &e) {
        // Will emit warnings, but we do not care about them much…
        // This should not be reachable anyway.
        g_task_return_new_error(task, 0, 0, "%s", e.what());
    }
}

void
next_async_callback(GObject *, GAsyncResult *res, void *data)
{
    auto slotptr = static_cast<Gio::SlotAsyncReady *>(data);
    std::unique_ptr<Gio::SlotAsyncReady> slot(slotptr);

    try {
        auto result = Glib::wrap(res, false);
        (*slotptr)(result);
    } catch (...) {
        Glib::exception_handlers_invoke();
    }
}

void iterate_cb2(GObject *obj, GAsyncResult *res, void *data)
{
    auto pair = reinterpret_cast<std::pair<Statement *, Gio::SlotAsyncReady *>*>(data);
    auto taskptr = reinterpret_cast<GTask *>(res);
    try {
        auto result = Glib::wrap(res, false);
        (*pair->second)(result);
        iterate_cb(pair, g_task_get_cancellable(taskptr));
    } catch (StatementDone) {
    } catch (...) {
        Glib::exception_handlers_invoke();
    }
}

void iterate_cb(std::pair<Statement *, Gio::SlotAsyncReady *> *pair,
                GCancellable *c)
{
    GTask* task = g_task_new(pair->first->gobj(), c,
                             iterate_cb2, pair);
    pair->first->schedule_fn([pair, task](){
        return get_next_row(pair->first, task);
    });
}

void
Statement::next_async(Gio::SlotAsyncReady cb)
{
    return this->next_async(cb, Glib::RefPtr<Gio::Cancellable>(nullptr));
}

void
Statement::next_async(Gio::SlotAsyncReady cb,
                      const Glib::RefPtr<Gio::Cancellable>& c)
{
    Gio::SlotAsyncReady *copy = new Gio::SlotAsyncReady(cb);
    GTask* task = g_task_new(this->gobj(), Glib::unwrap(c),
                             next_async_callback, copy);
    this->schedule_fn([this, task](){
        return get_next_row(this, task);
    });
}

Row *
Statement::next_finish(Statement::result_t &res)
{
    GError *error = nullptr;
    auto task = reinterpret_cast<GTask *>(Glib::unwrap(res));
    auto rowptr = static_cast<Row *>(g_task_propagate_pointer(task, &error));
    if(rowptr == nullptr)
        throw StatementDone();
    if(error)
        Glib::Error::throw_exception(error);
    return rowptr;
}

void
Statement::iterate(Gio::SlotAsyncReady slot)
{
    Statement::iterate(slot, Glib::RefPtr<Gio::Cancellable>(nullptr));
}

void
Statement::iterate(Gio::SlotAsyncReady cb,
                   const Glib::RefPtr<Gio::Cancellable> &c)
{
    Gio::SlotAsyncReady *copy = new Gio::SlotAsyncReady(cb);
    auto pair = new std::pair<Statement *, Gio::SlotAsyncReady *>(this, copy);
    iterate_cb(pair, Glib::unwrap(c));
}

Row *
Statement::iteration_finish(Statement::result_t &res)
{
    GError *error = nullptr;
    auto task = reinterpret_cast<GTask *>(Glib::unwrap(res));
    auto rowptr = static_cast<Row *>(g_task_propagate_pointer(task, &error));
    if(rowptr == nullptr)
        throw StatementDone();
    if(error)
        Glib::Error::throw_exception(error);
    return rowptr;
}
