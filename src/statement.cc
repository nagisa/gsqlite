#include <sstream>

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
