#include "connection.hh"

Connection::Connection(const char *filename)
    : Glib::ObjectBase(typeid(Connection))
{
    int ret = SQLITE_OK;
    if((ret = sqlite3_open(filename, &this->handle)) != SQLITE_OK)
        throw SQLiteException(ret);
    this->thread = Glib::Threads::Thread::create([this](){
        this->worker();
    });
}

Connection::~Connection()
{
    int ret = SQLITE_ERROR;
    // Finalize the worker thread. This makes the destructor
    // a potentially time intensive task.
    this->add_job([](){
        throw Glib::Threads::Thread::Exit();
    });
    this->thread->join();

    ret = sqlite3_close(this->handle);
    if(ret != SQLITE_OK) {
        throw SQLiteException(ret);
        // TODO: Well, dang. Apparently one should never throw from destructors.
        // Maybe log an error or something?
    }
    this->thread = NULL;
    this->handle = NULL;
}

Statement*
Connection::prepare(const char *query, int nByte, const char **tail)
{
    sqlite3_stmt *stmt;
    int err = sqlite3_prepare_v2(this->handle, query, nByte, &stmt, tail);
    if(err != SQLITE_OK){
        throw SQLiteException(err);
    }

    // Keep a pointer to self in every statement so the connection
    // cannot be destroyed before all the statements are finalized.
    std::shared_ptr<Connection> self(this->self);
    return new Statement{stmt, [self](jobfn_t job){
        self->add_job(job);
    }};
}

void
Connection::worker()
{
    while(true){
        jobfn_t job;
        {
            Glib::Threads::Mutex::Lock lock(this->queue_mtx);
            while(this->queue.empty())
                this->queue_push.wait(this->queue_mtx);
            job = this->queue.front();
            this->queue.pop();
        }
        job();
    }
}

void
Connection::add_job(jobfn_t job)
{
    {
        Glib::Threads::Mutex::Lock lock(this->queue_mtx);
        this->queue.push(job);
    }
    this->queue_push.signal();
}

std::shared_ptr<Connection>
Connection::create(const char *filename)
{
    std::shared_ptr<Connection> c(new Connection(filename));
    c->self = c;
    return std::move(c);
}
