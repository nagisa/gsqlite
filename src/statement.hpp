#ifndef _GSQLITE_STATEMENT
#define _GSQLITE_STATEMENT

#include <future>

#include "sqlite_common.hpp"
#include "row.hpp"



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

        /// Retrieve a next row from the list of query results. Non-blocking.
        /// Will throw an exception when there’s no more results as well as
        /// when an error occurs. Call to next() invalidates any Row object
        /// retrieved by this Statement.
        // TODO: ensure that invalidated row cannot be used.
        std::future<std::shared_ptr<Row>> next();

        std::string show() override;
    private:
        sqlite3_stmt *statement;
        std::weak_ptr<Row> last_row;
        std::function<void (jobfn_t)> schedule_fn;
};

#endif
