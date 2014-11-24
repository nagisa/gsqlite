#include "error.hh"
#include "connection.hh"

Glib::Quark
SQLiteError::quark = Glib::Quark("SQLITEMM");

bool
SQLiteError::registered = false;

void
SQLiteError::raise(GError *e)
{
    throw SQLiteError(e);
}

#define REGISTER_ERROR if(!SQLiteError::registered){\
    Glib::Error::register_domain(SQLiteError::quark.id(),\
                                 SQLiteError::raise);\
    SQLiteError::registered = true;\
}

SQLiteError::SQLiteError(const Connection& c)
    : Glib::Error(SQLiteError::quark.id(), c.error_code(),
                  c.error_message())
{
    REGISTER_ERROR
}

SQLiteError::SQLiteError(const int code)
    : Glib::Error(SQLiteError::quark.id(), code,
                  SQLiteError::message_for_code(code))
{
    REGISTER_ERROR
}

SQLiteError::SQLiteError(GError *e)
    : Glib::Error(e)
{
    REGISTER_ERROR
}

#undef REGISTER_ERROR

SQLiteError::~SQLiteError()
{
}

Glib::ustring
SQLiteError::message_for_code(int code)
{
    if(code >= 0)
        return {sqlite3_errstr(code)};
    else switch (code) {
        case _SQLITE_NULL_STATEMENT:
            return {"query is empty or miscellaneous error has occurred"};
        case _SQLITE_ROW_ERROR:
            return {"row initialized without values"};
        case _SQLITE_VALUE_TYPE_ERROR:
            return {"value type does not match constructor"};
        case _SQLITE_NULL_EXTRACT:
            return {"extract of null column"};
        default:
            return {"unknown binding error"};
    }
}

bool
SQLiteError::binding_error()
{
    return this->code() < 0;
}
