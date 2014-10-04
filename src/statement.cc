#include <sstream>
#include <vector>

#include "statement.hpp"

Statement::Statement(std::function<void (jobfn_t)> sched, sqlite3_stmt *stmt)
{
    if(stmt == NULL){
        throw SQLiteException(_SQLITE_NULL_STMT);
    }
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

std::vector<std::unique_ptr<Value>> collect_cols(int cols, sqlite3_stmt* stmt){
    std::vector<std::unique_ptr<Value>> vec(cols);
    for(int i = 0; i < cols; i += 1){
        vec[i] = std::unique_ptr<Value>(
            mkValue(sqlite3_column_value(stmt, i))
        );
    }
    return vec;
}

std::future<std::shared_ptr<Row>>
Statement::next()
{
    // Invalidate previous row, if any.
    if(!this->last_row.expired()){
        this->last_row.lock()->invalidate();
    }

    auto promise = new std::promise<std::shared_ptr<Row>>;
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
                    auto row = std::make_shared<Row>(std::move(vals));
                    this->last_row = row;
                    promise->set_value(row);
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
