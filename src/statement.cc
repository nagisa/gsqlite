#include <sstream>
#include <vector>

#include "statement.hpp"

Statement::Statement(std::function<void (jobfn_t)> sched, sqlite3_stmt *stmt)
{
    if(stmt == NULL) throw SQLiteException(_SQLITE_NULL_STMT);
    this->statement = stmt;

    this->schedule_fn = sched;
}

Statement::~Statement()
{
    sqlite3_finalize(this->statement);
}

std::string
Statement::show()
{
    std::stringstream ss;
    ss << "Statement() for " << sqlite3_sql(this->statement);
    return ss.str();
}

std::vector<sqlite3_value *> collect_cols(int cols, sqlite3_stmt* stmt){
    std::vector<sqlite3_value *> vec(cols);
    for(int i = 0; i < cols; i += 1){
        vec[i] = sqlite3_column_value(stmt, i);
    }
    return vec;
}

std::future<Row>
Statement::next()
{
    auto promise = new std::promise<Row>;
    this->schedule_fn([&, promise](){
        try {
            int ret = SQLITE_BUSY;
            while((ret = sqlite3_step(this->statement)) == SQLITE_BUSY);
            switch(ret) {
                case SQLITE_DONE:
                    throw std::future_error(std::future_errc::broken_promise);
                    break;
                case SQLITE_ROW:
                    {
                    const int cols = sqlite3_column_count(this->statement);
                    auto vals = collect_cols(cols, this->statement);
                    promise->set_value(std::move(Row(std::move(vals))));
                    }
                    break;
                default:
                    throw SQLiteException(ret);
                    break;
            }
        } catch (...) {
            try {
                promise->set_exception(std::current_exception());
            } catch (...) {}
        }
        delete promise;
    });
    auto future = promise->get_future();
    return future;
}
