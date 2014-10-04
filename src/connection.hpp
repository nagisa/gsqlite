#pragma once

#include <sqlite3.h>
#include <queue>
#include <glibmm/threads.h>

#include "interfaces.hpp"
#include "statement.hpp"
#include "sqlite_common.hpp"

/// An open database connection.

/// An open database connection. This class’ methods are thread-safe and
/// non-blocking (except for construction and destruction). Construction may
/// throw an exception.
class Connection : public Showable {
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
        Connection(const char *filename);
        Connection(const char *filename, int flags, const char *zVfs);
        ~Connection();

        /// Prepare a Statement on an open SQLite database.
        /// See: www.sqlite.org/c3ref/prepare.html
        Statement* prepare(const char *query, int nByte = -1,
                           const char **tail = NULL);
        /// Prepare a Statement on an open SQLite database.
        /// See: www.sqlite.org/c3ref/prepare.html
        Statement* prepare(const char *query, const char **tail);
        /// Prepare a Statement on an open SQLite database.
        /// See: www.sqlite.org/c3ref/prepare.html
        Statement* prepare16(const char *query, int nByte = -1,
                             const char **tail = NULL);
        /// Prepare a Statement on an open SQLite database.
        /// See: www.sqlite.org/c3ref/prepare.html
        Statement* prepare16(const char *query, const char **tail);

        std::string show() override;

    private:
        void worker();
        void add_job(jobfn_t);
};
