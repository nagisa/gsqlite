#pragma once
#include <sqlite3.h>
#include <giomm.h>

// Forward declarations
class Row;
class Connection;

/// @brief A query to a previously opened database.
///
/// This class’ methods are thread-safe and non-blocking. Construction may
/// throw an exception.  This class is not meaningfully constructable by an end
/// API user. The only way to retrieve a meaningful instance of the class is
/// through Connection::prepare.
class Statement : Glib::Object {
    public:
    typedef std::function<void (std::function<void ()>)> schedule_fn_t;
    typedef std::shared_ptr<Connection> connection_ptr_t;
    typedef Glib::RefPtr<Gio::AsyncResult> result_t;

    protected:
    sqlite3_stmt *statement;
    schedule_fn_t schedule_fn;
    connection_ptr_t connection;

    public:
    Statement(sqlite3_stmt *, connection_ptr_t, schedule_fn_t);
    Statement(Statement &&);
    Statement(const Statement &) = delete;
    ~Statement();

    /// Return the number of columns in the result set returned by the
    /// prepared statement. This routine returns 0 if the Statement is an
    /// SQL statement that does not return data (for example an UPDATE).
    int columns();

    const char *column_name(const int n);

    /// Determine the name of table column that is the origin
    /// of a particular result column in SELECT statement.
    ///
    /// @param n result column returned by the query. The left-most column
    /// is 0.
    const char *origin_name(const int n);

    /// Determine the type of result column.
    ///
    /// @param n result column returned by the query. The left-most column
    /// is 0.
    int column_type(const int n);

    /// Determine the declared type of result column.
    ///
    /// @param n result column returned by the query. The left-most column
    /// is 0.
    const char* column_decltype(const int n);

    /// @brief Retrieve next result row asynchronously.
    void next_async(Gio::SlotAsyncReady);

    /// @copydoc next_async()
    void next_async(Gio::SlotAsyncReady,
                    const Glib::RefPtr<Gio::Cancellable> &);

    /// @brief Finish the next_async() operation and retrieve the resulting
    /// Row.
    ///
    /// Will return nullptr when there’s no more rows available (see
    /// [SQLITE_DONE]).
    /// [SQLITE_DONE]: https://www.sqlite.org/rescode.html#done
    Row *next_finish(Statement::result_t &);

    /// A convenience function to iterate through all the rows calling
    /// the callback for each row. Cancelling the Gio::Cancellable
    /// will stop the iteration, though the callback might still be called
    /// a few times with a genuine result after the cancellation occurs.
    ///
    /// Call iteration_finish() inside the callback to retrieve a Row.
    ///
    /// Does not block.
    void iterate(Gio::SlotAsyncReady);

    void iterate(Gio::SlotAsyncReady,
                 const Glib::RefPtr<Gio::Cancellable> &);

    /// @brief Retrieve row for current iteration.
    Row *iteration_finish(Statement::result_t &);

    friend void get_next_row(Statement *stmt, GTask *task);
    friend void iterate_cb(std::pair<Statement *, Gio::SlotAsyncReady *> *,
                           GCancellable *);
};
