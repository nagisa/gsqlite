#include <sstream>

#include "sqlite_common.hpp"

SQLiteException::SQLiteException(int code)
{
    this->code = code;
}


const char *
SQLiteException::what() const throw()
{
    if(this->code >= 0) return sqlite3_errstr(this->code);
    else switch(this->code) {
        case _SQLITE_NULL_STMT:
            return "query is empty or miscellaneous error has occurred";
        default:
            return "unknown error";
    }
}


std::string
SQLiteException::show()
{
    std::stringstream ss;
    ss << "SQLiteException(" << this->code << ")";
    return ss.str();
}