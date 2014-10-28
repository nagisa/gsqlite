#pragma once
#include <glibmm.h>

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


#define _SQLITE_NULL_STATEMENT -1
#define _SQLITE_ROW_ERROR      -2
#define _SQLITE_VALUE_TYPE     -3
#define _SQLITE_NULL_EXTRACT   -4
