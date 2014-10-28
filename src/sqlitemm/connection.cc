#include "error.hh"
#include "connection.hh"
#include "statement.hh"

Connection::Connection(const char *filename)
    : Glib::ObjectBase(typeid(Connection))
    , handle(nullptr)
{
    int ret = SQLITE_OK;
    if((ret = sqlite3_open(filename, &this->handle)) != SQLITE_OK)
        throw SQLiteError(ret);
    this->thread = Glib::Threads::Thread::create([this](){
        this->worker();
    });
}

std::shared_ptr<Connection>
Connection::create(const char *filename)
{
    std::shared_ptr<Connection> c(new Connection(filename));
    c->self = c;
    return std::move(c);
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
        // In case of sound API this should never happen.
        throw SQLiteError(*this);
    }
    this->thread = NULL;
    this->handle = NULL;
}

Statement*
Connection::prepare(const char *query, int nByte, const char **tail)
{
    sqlite3_stmt *stmt;
    int err = sqlite3_prepare_v2(this->handle, query, nByte, &stmt, tail);
    if(err != SQLITE_OK)
        throw SQLiteError(*this);

    // Keep a pointer to self in every statement so the connection
    // cannot be destroyed before all the statements are finalized.
    std::shared_ptr<Connection> self(this->self);
    return new Statement{stmt, self, [this](jobfn_t job){
        this->add_job(job);
    }};
}

int
Connection::error_code() const
{
    return sqlite3_errcode(this->handle);
}

Glib::ustring
Connection::error_message() const
{
    return { sqlite3_errmsg(this->handle) };
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
