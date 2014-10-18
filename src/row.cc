#include <sstream>
#include "row.hh"
#include "sigc_common.hh"

#define INVALID_WHAT "row is not resolved yet"

Glib::PropertyProxy_ReadOnly<Row::state_t>
Row::state(){
    return Glib::PropertyProxy_ReadOnly<Row::state_t>(this, "state");
}

Row::Row()
    : Glib::ObjectBase(typeid(Row))
    , Glib::Object()
    , _exception(*this, "exception", nullptr)
    , _state(*this, "state", PRISTINE)
{
};

Row::Row(Row::values_t values) : Row()
{
    if(values.empty()){
        throw SQLiteException(_SQLITE_ROW_ERR);
    }
    this->values = std::move(values);
}

Row::~Row()
{
}

size_t
Row::columns()
{
    this->ensure_resolved();
    return this->values.size();
}

Value *
Row::operator[](size_t n)
{
    this->ensure_resolved();
    return this->values[n].get();
};

#define GENERATE_EXTRACT(type, index) {\
    auto ptr = (*this)[(index)];\
    if(ptr == nullptr)\
        throw SQLiteException(_SQLITE_NULL_EXTRACT);\
    auto res = dynamic_cast<type>(ptr);\
    if(res == nullptr)\
        throw std::bad_cast();\
    return **res;\
}

template<> int64_t
Row::extract(size_t n)
{
    GENERATE_EXTRACT(IntValue *, n);
}

template<> double
Row::extract(size_t n)
{
    GENERATE_EXTRACT(FloatValue *, n);
}

template<> const void *
Row::extract(size_t n)
{
    GENERATE_EXTRACT(BlobValue *, n);
}

template<> const unsigned char *
Row::extract(size_t n)
{
    GENERATE_EXTRACT(TextValue *, n);
}

#undef GENERATE_EXTRACT

#include <iostream>
void
Row::ensure_resolved()
{
    if(this->_state.get_value() != RESOLVED)
        throw std::logic_error(INVALID_WHAT);
}

std::string
Row::show()
{
    std::stringstream ss;
    ss << "Row(" << &this->values << ")";
    switch(this->_state.get_value()) {
        case PRISTINE:
            ss << "::pristine";
            break;
        case FAILED:
            ss << "::failed";
            break;
        case DONE:
            ss << "::done";
            break;
        default:
            break;
    }
    return ss.str();
}

void
Row::resolve(Row::values_t vals)
{
    if(vals.empty()){
        throw SQLiteException(_SQLITE_ROW_ERR);
    }
    this->freeze_notify();
    if(this->_state.get_value() != Row::PRISTINE)
        throw std::logic_error("row not pristine anymore");
    this->values = std::move(vals);
    this->_state.set_value(Row::RESOLVED);
    Glib::signal_idle().connect([&](){
        this->thaw_notify();
        return false;
    });
}

void
Row::fail(std::exception_ptr p)
{
    this->freeze_notify();
    this->_exception.set_value(p);
    this->_state.set_value(Row::FAILED);
    Glib::signal_idle().connect([&](){
        this->thaw_notify();
        return false;
    });
}

void
Row::monitor(Row::monitor_fn_t fn)
{
    auto cc = new sigc::connection;
    auto c = this->state().signal_changed().connect([this, fn, cc](){
        auto ret = fn(this->_state.get_value());
        if(!ret) cc->disconnect();
        delete cc;
    });
    *cc = c;
}
