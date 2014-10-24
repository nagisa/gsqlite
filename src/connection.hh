#pragma once

#include <sqlite3.h>
#include <queue>
#include <glibmm/threads.h>
#include <glibmm/object.h>

#include "interfaces.hh"
#include "statement.hh"
#include "sqlite_common.hh"

/// @brief An open database connection.
///
/// This class’ methods are thread-safe and non-blocking (except for
/// construction and destruction).
class Connection : public Glib::Object {

    private:
        std::weak_ptr<Connection>   self;
        sqlite3 *handle = NULL;
        Glib::Threads::Mutex        queue_mtx;
        Glib::Threads::Cond         queue_push;
        Glib::Threads::Thread       *thread;
        std::queue<jobfn_t>         queue;

    private:
        Connection(const char *filename);
    public:
        Connection(const Connection&) = delete;

        /// [Close] a Connection to a SQLite database.
        /// [Close]: http://www.sqlite.org/c3ref/close.html
        ///
        /// This destructor might throw an exception in case database
        /// cannot be closed properly. For example all Statements prepared
        /// for this connection must be destroyed before the Connection itself.
        ~Connection();

        /// [Prepare] a Statement on an open SQLite database.
        /// [Prepare]: www.sqlite.org/c3ref/prepare.html
        Statement* prepare(const char *query, int nByte = -1,
                           const char **tail = NULL);

        /// [Open] a Connection to a SQLite database.
        /// [Open]: http://www.sqlite.org/c3ref/open.html
        ///
        /// This function uses v1 interface.
        static std::shared_ptr<Connection> create(const char *filename);

        /// Get a last error that occured on this connection.
        ///
        /// These errors are potentially most accurate available.
        friend SQLiteException getError(Connection&);
    private:
        void worker();
        void add_job(jobfn_t);
};
