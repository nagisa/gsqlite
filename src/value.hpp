#pragma once
#include <cinttypes>
#include "sqlite_common.hpp"

/// Interface which makes polymorphism possible between different types of
/// SQL values.
class Value : public Showable {
    public:
        virtual ~Value(){};
        virtual int getType() = 0;
};

#define GEN_VALUE(name, derive, type_enum, type) \
class name : public derive {\
    public:\
        name(sqlite3_value *);\
        virtual ~name();\
        int getType() override { return (type_enum); };\
        virtual type operator*() { return this->value; }; \
        std::string show() override { return #name"()"; }; \
    private:\
        type value; \
}

GEN_VALUE(IntValue, Value, SQLITE_INTEGER, int64_t);
GEN_VALUE(FloatValue, Value, SQLITE_FLOAT, double);
GEN_VALUE(BlobValue, Value, SQLITE_BLOB, const void *);
GEN_VALUE(TextValue, Value, SQLITE_TEXT, const unsigned char *);

/// Generate a thread-safe Value* from sqlite3_value.
/// To actually retrieve the value it one should use dynamic_cast.
Value *mkValue(sqlite3_value *);
