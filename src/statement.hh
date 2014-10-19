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
