#pragma once

#include "sqlite_common.hh"
#include "row.hh"

/// @brief A query to a previously opened database.
///
/// This class’ methods are thread-safe and non-blocking. Construction may
/// throw an exception.  This class is not meaningfully constructable by an end
/// API user. The only way to retrieve a meaningful instance of the class is
/// through Connection::prepare.
class Statement : public Showable {
    public:
        typedef std::shared_ptr<Row> result_t;

    private:
        sqlite3_stmt *statement;
        std::function<void (jobfn_t)> schedule_fn;

    public:
        Statement(std::function<void (jobfn_t)>, sqlite3_stmt *);
        ~Statement();

        /// Return the number of columns in the result set returned by the
        /// prepared statement. This routine returns 0 if the Statement is an
        /// SQL statement that does not return data (for example an UPDATE).
        int columns();

        /// Determine the name of table column that is the origin
        /// of a particular result column in SELECT statement.
        ///
        /// @param n result column returned by the query. The left-most column
        /// is 0.
        const char *origin_name(int n);

        /// Determine the type of result column.
        ///
        /// @param n result column returned by the query. The left-most column
        /// is 0.
        int column_type(int n);

        /// Determine the declared type of result column.
        ///
        /// @param n result column returned by the query. The left-most column
        /// is 0.
        const char* column_decltype(int n);

        /// Retrieve a next row from the list of query results. Non-blocking.
        /// Will throw an exception when there’s no more results as well as
        /// when an error occurs. Call to next() invalidates any previous Row
        /// object retrieved by this Statement.
        ///
        /// Next Row returned after the last actual result row will have
        /// state property set to Row::DONE. Calling next() after such
        /// Row is undefined.
        result_t next();

        /// A convenience function to iterate through all the rows calling
        /// the callback for each row. Row is guaranteed to be non-pristine
        /// inside the callback. If callback returns false, iteration is halted
        /// and subsequent call to iterate() or next() will return row after
        /// the point where the iteration was last halted. Iteration will also
        /// be halted if Row with Row::status() != Row::RESOLVED is
        /// encountered.
        ///
        /// Does not block. All callbacks will be invoked in thread which runs
        /// Glib::MainLoop.
        void iterate(std::function<bool (result_t)>);

        std::string show() override;
};
