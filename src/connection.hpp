#ifndef _GSQLITE_CONNECTION
#define _GSQLITE_CONNECTION

#include <sqlite3.h>
#include <queue>
#include <functional>
#include <glibmm/threads.h>

#include "interfaces.hpp"
#include "sqlite_common.hpp"


/// Class representing an open database connection.

/// Class representing an open database connection. Calls to this class
/// are thread-safe and non-blocking (except for construction and destruction).
/// Construction might throw an exception.
class Connection : public Showable {
    public:
        Connection(const char *filename);
        Connection(const char *filename, int flags, const char *zVfs);
        virtual ~Connection();

        virtual std::string show();

    private:
        sqlite3 *handle = NULL;
        int type = 0;

        // TODO: Probably should be abstracted out to a struct or something.
        Glib::Threads::Mutex   queue_mtx;
        Glib::Threads::Cond    queue_push;
        Glib::Threads::Thread *thread;
        // Lets implement monads, fix the syntax and Haskell is not
        // too far away! Glory to Artotzka!
        std::queue<std::function<void ()>> queue;
        void worker();
};


#endif
