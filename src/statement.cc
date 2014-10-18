#include <sstream>
#include <vector>
#include <utility>

#include "statement.hh"

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

Statement::result_t
Statement::next()
{
    Statement::result_t res(new Row());
    this->schedule_fn([this, res](){
        int ret = SQLITE_BUSY;
        while((ret = sqlite3_step(this->statement)) == SQLITE_BUSY);
        try {
        switch(ret) {
            case SQLITE_DONE:
                res->set_property("state", Row::DONE);
                break;
            case SQLITE_ROW:
                {
                const int cols = sqlite3_column_count(this->statement);
                auto vals = collect_cols(cols, this->statement);
                res->resolve(std::move(vals));
                }
                break;
            default:
                throw SQLiteException(ret);
                break;
        }
        } catch (...) {
            res->fail(std::current_exception());
        }
    });
    return res;
}
