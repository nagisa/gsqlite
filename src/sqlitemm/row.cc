#include "private/sigc_fix.hh"
#include "error.hh"
#include "row.hh"
#include "value.hpp"

Row::Row(Row::values_t values)
    : Glib::ObjectBase(typeid(Row))
    , Glib::Object()
    , values(std::move(values))
{
    if(this->values.empty()){
        throw SQLiteError(_SQLITE_ROW_ERROR);
    }
}

Row::~Row()
{
}

size_t
Row::columns()
{
    return this->values.size();
}

Value *
Row::operator[](const size_t n)
{
    return this->values[n].get();
};

Value *
Row::steal(const size_t n)
{
    return this->values[n].release();
}

#define GENERATE_EXTRACT(type, index) {\
    auto ptr = (*this)[(index)];\
    if(ptr == nullptr)\
        throw SQLiteError(_SQLITE_NULL_EXTRACT);\
    auto res = dynamic_cast<type>(ptr);\
    if(res == nullptr)\
        throw std::bad_cast();\
    return **res;\
}

template<> int64_t
Row::extract(const size_t n)
{
    GENERATE_EXTRACT(IntValue *, n);
}

template<> double
Row::extract(const size_t n)
{
    GENERATE_EXTRACT(FloatValue *, n);
}

template<> const void *
Row::extract(const size_t n)
{
    GENERATE_EXTRACT(BlobValue *, n);
}

template<> const unsigned char *
Row::extract(const size_t n)
{
    GENERATE_EXTRACT(TextValue *, n);
}

#undef GENERATE_EXTRACT
