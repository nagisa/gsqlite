#include <sstream>
#include "row.hpp"

#define INVALID_WHAT "row is invalid"

Row::Row(std::vector<std::unique_ptr<Value>> values)
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
    this->ensure_valid();
    return this->values.size();
}

Value *
Row::operator[](size_t n)
{
    this->ensure_valid();
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

void
Row::invalidate()
{
    this->invalid_ = true;
}

void
Row::ensure_valid()
{
    if(this->invalid_)
        throw std::logic_error(INVALID_WHAT);
}

std::string
Row::show()
{
    std::stringstream ss;
    ss << "Row(" << &this->values << ")";
    if(this->invalid_){
        ss << "::invalid";
    }
    return ss.str();
}
