#pragma once

#include <exception>
#include <functional>
#include <string>
#include <sqlite3.h>

#include "interfaces.hh"

typedef std::function<void ()> jobfn_t;

/// @brief Exception used to inform about SQLite errors.
///
/// An exception object thrown when underlying SQLite errors occur
/// (informational) or API misuse which might cause SQLite errors occur
/// (preventional).
class SQLiteException : public std::exception, public Showable {
    public:
        SQLiteException(int code);
        ~SQLiteException() noexcept {};

        /// Returns a string explaining the error.
        const char* what() const throw() override;
        /// Tells whether the error is informational (false) or preventional
        /// (true).
        bool liberr();

        std::string show() override;
    private:
        int code;
};


/// Preventional error codes
#define _SQLITE_NULL_STMT -1
#define _SQLITE_ROW_ERR -2
#define _SQLITE_VALUE_TYPE -3
#define _SQLITE_NULL_EXTRACT -4
