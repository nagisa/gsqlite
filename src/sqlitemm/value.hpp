#pragma once
#include <glibmm.h>
#include <cinttypes>
#include <sqlite3.h>

/// @brief Objects representing SQLite values.
class Value {
    public:
        virtual ~Value(){};

        /// Get the type of value object. This method should return one of
        /// the [fundamental SQLite type enumeration
        ///  values](http://www.sqlite.org/c3ref/c_blob.html).
        virtual int getType() = 0;
};

#define GEN_VALUE(name, derive, type_enum, type) \
class name : public derive {\
    private:\
        type value;\
    public:\
        name(sqlite3_value *);\
        virtual ~name();\
        int getType() override { return (type_enum); };\
        /** Dereference the object into POD type stored inside */ \
        virtual type operator*() { return this->value; }; \
}

/// Values of SQLITE_INTEGER fundamental SQLite data type.
GEN_VALUE(IntValue, Value, SQLITE_INTEGER, int64_t);

/// Values of SQLITE_FLOAT fundamental SQLite data type.
GEN_VALUE(FloatValue, Value, SQLITE_FLOAT, double);

/// Values of SQLITE_BLOB fundamental SQLite data type.
GEN_VALUE(BlobValue, Value, SQLITE_BLOB, std::string);

/// Values of SQLITE_TEXT fundamental SQLite data type. UTF-8 encoded.
GEN_VALUE(TextValue, Value, SQLITE_TEXT, Glib::ustring);

#undef GEN_VALUE

/// Generate a thread-safe Value* from sqlite3_value.
Value *mkValue(sqlite3_value *);
