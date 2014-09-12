#include <sstream>

#include "sqlite_common.hpp"

SQLiteException::SQLiteException(int code)
{
    this->code = code;
}


const char *
SQLiteException::what() const throw()
{
    return sqlite3_errstr(this->code);
}


std::string
SQLiteException::show()
{
    std::stringstream ss;
    ss << "SQLiteException(" << this->code << ")";
    return ss.str();
}
