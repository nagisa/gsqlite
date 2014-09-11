#include <sstream>

#include "connection.hpp"

void Connection::worker(){
    // TODO: actually pull down jobs from the queue and execute them, maybe?
    throw Glib::Threads::Thread::Exit();
}

Connection::Connection(const char *filename)
{
    int ret;
    if((ret = sqlite3_open(filename, &this->handle)) != SQLITE_OK)
        throw SQLiteException(ret);

    this->thread = Glib::Threads::Thread::create([&](){ worker(); });
    this->type = 1;
}

Connection::Connection(const char *filename, int flags, const char *zVfs)
{
    int ret;
    if((ret = sqlite3_open_v2(filename, &this->handle, flags, zVfs)))
        throw SQLiteException(ret);

    this->thread = Glib::Threads::Thread::create([&](){ worker(); });
    this->type = 2;
}

Connection::~Connection()
{
    int ret;
    this->thread->join();
    if(this->type == 1) ret = sqlite3_close(this->handle);
    if(this->type == 2) ret = sqlite3_close_v2(this->handle);
    if(ret != SQLITE_OK) throw SQLiteException(ret);
    this->handle = NULL;
}

std::string
Connection::show()
{
    std::stringstream ss;
    ss << "Connection()";
    return ss.str();
}
