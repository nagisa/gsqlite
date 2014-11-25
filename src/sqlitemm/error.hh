#pragma once
#include <glibmm.h>

#define _SQLITE_NULL_STATEMENT   -1
#define _SQLITE_ROW_ERROR        -2
#define _SQLITE_VALUE_TYPE_ERROR -3
#define _SQLITE_NULL_EXTRACT     -4
#define _SQLITE_STATEMENT_DONE   -5

// Forward declaration
class Connection;

/// @brief Errors arising from underlying SQLite library
class SQLiteError : public Glib::Error {
    static bool registered;
    public:
        static Glib::Quark quark;
        SQLiteError(const Connection&);
        SQLiteError(const int);
        SQLiteError(GError *);
        ~SQLiteError();

        bool binding_error();
        static void raise(GError *);
    private:
        static Glib::ustring message_for_code(int code);
};

class SQLiteNullStatement : public SQLiteError {
    public:
        SQLiteNullStatement() : SQLiteError(_SQLITE_NULL_STATEMENT){};
};

class SQLiteRowError : public SQLiteError {
    public:
        SQLiteRowError() : SQLiteError (_SQLITE_ROW_ERROR){};
};

class SQLiteValueTypeError : public SQLiteError {
    public:
        SQLiteValueTypeError() : SQLiteError(_SQLITE_VALUE_TYPE_ERROR){};
};

class SQLiteNullExtract : public SQLiteError {
    public:
        SQLiteNullExtract() : SQLiteError(_SQLITE_NULL_EXTRACT){};
};

class SQLiteStatementDone : public SQLiteError {
    public:
    SQLiteStatementDone() : SQLiteError(_SQLITE_STATEMENT_DONE){};
};
