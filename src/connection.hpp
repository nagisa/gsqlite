#ifndef _GSQLITE_CONNECTION
#define _GSQLITE_CONNECTION

#include <sqlite3.h>

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

};


#endif
