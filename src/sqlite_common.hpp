#ifndef _GSQLITE_SQLITE_COMMON
#define _GSQLITE_SQLITE_COMMON

#include <exception>
#include <functional>
#include <string>
#include <sqlite3.h>

#include "interfaces.hpp"

typedef std::function<void ()> jobfn_t;

class SQLiteException : public std::exception, public Showable {
    public:
        SQLiteException(int code);
        ~SQLiteException() noexcept {};

        const char* what() const throw() override;
        std::string show() override;
    private:
        int code;
};

// Custom error codes
#define _SQLITE_NULL_STMT -1


#endif
