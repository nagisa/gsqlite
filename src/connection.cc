#include <sstream>

#include "connection.hh"


Connection::Connection(const char *filename)
{
    int ret;
    if((ret = sqlite3_open(filename, &this->handle)) != SQLITE_OK)
        throw SQLiteException(ret);

    this->thread = Glib::Threads::Thread::create([&](){ this->worker(); });
    this->type = 1;
}

Connection::Connection(const char *filename, int flags, const char *zVfs)
{
    int ret;
    if((ret = sqlite3_open_v2(filename, &this->handle, flags, zVfs)))
        throw SQLiteException(ret);

    this->thread = Glib::Threads::Thread::create([&](){ this->worker(); });
    this->type = 2;
}

Connection::~Connection()
{
    int ret = SQLITE_ERROR;
    this->add_job([](){ throw Glib::Threads::Thread::Exit(); });
    this->thread->join();
    if(this->type == 1){
        ret = sqlite3_close(this->handle);
    } else if(this->type == 2){
        ret = sqlite3_close_v2(this->handle);
    }
    if(ret != SQLITE_OK) {
        throw SQLiteException(ret);
        // TODO: Well, dang. Apparently one should never throw from destructors.
        // Maybe log an error or something?
    }
    this->thread = NULL;
    this->handle = NULL;
}

std::string
Connection::show()
{
    // Keeping filename around just for this is wasteful;
    // So we’ll just give away Connection() here.
    return std::string("Connection()");
}

Statement *
Connection::prepare(const char *query, int nByte, const char **tail){
    sqlite3_stmt *stmt;
    int err = sqlite3_prepare_v2(this->handle, query, nByte, &stmt, tail);
    if(err != SQLITE_OK){
        throw SQLiteException(err);
    }
    return new Statement([&](jobfn_t job){
        this->add_job(job);
    }, stmt);
}

Statement *
Connection::prepare(const char *query, const char **tail){
    return this->prepare(query, -1, tail);
}

Statement *
Connection::prepare16(const char *query, int nByte, const char **tail){
    sqlite3_stmt *stmt;
    int err = sqlite3_prepare16_v2(this->handle, query, nByte, &stmt,
                                   reinterpret_cast<const void **>(tail));
    if(err != SQLITE_OK){
        throw SQLiteException(err);
    }
    return new Statement([&](jobfn_t job){
        this->add_job(job);
    }, stmt);
}

Statement *
Connection::prepare16(const char *query, const char **tail){
    return this->prepare16(query, -1, tail);
}

void Connection::worker(){
    while(true){
        jobfn_t job;
        {
            Glib::Threads::Mutex::Lock lock(this->queue_mtx);
            while(this->queue.empty()) {
                this->queue_push.wait(this->queue_mtx);
            }
            job = this->queue.front();
            this->queue.pop();
        }
        job();
    }
}

void Connection::add_job(jobfn_t job){
    {
        Glib::Threads::Mutex::Lock lock(this->queue_mtx);
        this->queue.push(job);
    }
    this->queue_push.signal();
}
