#include <sstream>

#include "sqlite_common.hh"

SQLiteException::SQLiteException(int code)
{
    this->code = code;
}


const char *
SQLiteException::what() const throw()
{
    if(this->code >= 0){
        return sqlite3_errstr(this->code);
    }
    else switch(this->code) {
        case _SQLITE_NULL_STMT:
            return "query is empty or miscellaneous error has occurred";
        case _SQLITE_ROW_ERR:
            return "row initialized without values";
        case _SQLITE_VALUE_TYPE:
            return "value type does not match constructor";
        case _SQLITE_NULL_EXTRACT:
            return "extract of null column";
        default:
            return "unknown error";
    }
}

bool
SQLiteException::liberr()
{
    return this->code >= 0;
}
