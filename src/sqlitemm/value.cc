#include "error.hh"
#include "value.hpp"

IntValue::IntValue(sqlite3_value *val)
{
    if(sqlite3_value_type(val) != SQLITE_INTEGER)
        throw SQLiteValueTypeError();
    this->value = sqlite3_value_int64(val);
}

IntValue::~IntValue()
{
}


FloatValue::FloatValue(sqlite3_value *val)
{
    if(sqlite3_value_type(val) != SQLITE_FLOAT)
        throw SQLiteValueTypeError();
    this->value = sqlite3_value_double(val);
}

FloatValue::~FloatValue()
{
}


TextValue::TextValue(sqlite3_value *val)
{
    if(sqlite3_value_type(val) != SQLITE_TEXT)
        throw SQLiteValueTypeError();
    this->value = reinterpret_cast<const char *>(sqlite3_value_text(val));
}

TextValue::~TextValue()
{
}


BlobValue::BlobValue(sqlite3_value *val)
{
    if(sqlite3_value_type(val) != SQLITE_BLOB)
        throw SQLiteValueTypeError();
    int len = sqlite3_value_bytes(val);
    auto v = reinterpret_cast<const char *>(sqlite3_value_blob(val));
    this->value = std::string(v, len);
}

BlobValue::~BlobValue()
{
}


Value *
mkValue(sqlite3_value *val)
{
    switch(sqlite3_value_type(val)){
        case SQLITE_NULL:
            return nullptr;
        case SQLITE_INTEGER:
            return new IntValue(val);
        case SQLITE_FLOAT:
            return new FloatValue(val);
        case SQLITE_BLOB:
            return new BlobValue(val);
        case SQLITE_TEXT:
            return new TextValue(val);
        default:
            return nullptr;
    }
}
