#ifndef _GSQLITE_STATEMENT
#define _GSQLITE_STATEMENT

#include "sqlite_common.hpp"


/// A single query to a previously opened database.

/// A single query to a previously opened database. This class’ methods are
/// thread-safe and non-blocking. Construction may throw an exception.
/// This class is not meaningfully constructable by an end API user. The only
/// way to retrieve a meaningful instance of the class is through
/// Connection::prepare.
class Statement : public Showable {
    public:
        Statement(std::function<void (jobfn_t)>, sqlite3_stmt *);
        ~Statement();

        std::string show() override;

    private:
        sqlite3_stmt *statement;
        std::function<void (jobfn_t)> schedule_fn;
};

#endif
