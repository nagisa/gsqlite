#ifndef _GSQLITE_SQLITE_COMMON
#define _GSQLITE_SQLITE_COMMON

#include <exception>
#include <string>
#include <sqlite3.h>

#include "interfaces.hpp"


class SQLiteException : public std::exception, public Showable {
    public:
        SQLiteException(int code);
        virtual ~SQLiteException() noexcept {};
        virtual const char* what() const throw();
        virtual std::string show();
    private:
        int code;
};



#endif
