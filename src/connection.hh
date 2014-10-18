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
class Connection : public Showable, public Glib::Object {
    private:
        sqlite3 *handle = NULL;
        int type = 0;
        Glib::Threads::Mutex   queue_mtx;
        Glib::Threads::Cond    queue_push;
        Glib::Threads::Thread *thread;
        // Lets implement monads, fix the syntax and Haskell is not
        // too far away! Glory to Artotzka!
        std::queue<jobfn_t> queue;

    public:
        /// [Open] a Connection to a SQLite database.
        /// [Open]: http://www.sqlite.org/c3ref/open.html
        ///
        /// This function uses v1 interface.
        Connection(const char *filename);
        /// [Open] a Connection to a SQLite database.
        /// [Open]: http://www.sqlite.org/c3ref/open.html
        ///
        /// This function uses v2 interface.
        Connection(const char *filename, int flags, const char *zVfs);

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
        /// @copydoc Connection::prepare
        Statement* prepare(const char *query, const char **tail);
        /// @copydoc Connection::prepare
        Statement* prepare16(const char *query, int nByte = -1,
                             const char **tail = NULL);
        /// @copydoc Connection::prepare
        Statement* prepare16(const char *query, const char **tail);

        std::string show() override;
    private:
        void worker();
        void add_job(jobfn_t);
};
