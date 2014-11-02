#pragma once
#include <vector>
#include <memory>
#include <glibmm.h>

class Value;

/// @brief A single row from the database
///
/// All columns are stored in the memory
class Row : public Glib::Object {
    public:
        typedef std::vector<std::unique_ptr<Value>> values_t;
    private:
        values_t values;
    public:
        Row(values_t);
        ~Row();

        /// Get amount of columns in the row.
        size_t columns();

        /// Get value at column n (0-indexed)
        ///
        /// May return nullptr indicating that column is NULL.
        /// Will throw std::logic_error if the row is invalid.
        /// Value belongs to Row and should not be deleted.
        /// Warning: bounds are not checked, indexing over bounds is undefined.
        Value* operator[](const size_t n);

        /// Similar to Row::operator[]. The only difference being in
        /// this making subsequent operations on the the column
        /// invalid and give control over Value lifetime to the caller.
        Value* steal(const size_t n);

        /// Extract usable value at column n (0-indexed)
        ///
        /// Will throw if:
        /// 1. (std::bad_cast) requested value type does not match type stored
        ///    in db. i.e. you call exract<double> while we have IntValue.
        /// 2. (SQLiteException) column n is NULL;
        /// 3. (std::logic_error) row is invalid.
        ///
        /// Warning: bounds are not checked, indexing over bounds is undefined.
        ///
        /// Warning: variants of extract which return a pointer (non-POD)
        /// don’t copy, therefore extracted data will only live as long
        /// as Row from which the value is extracted.
        template<typename T> T extract(const size_t n);
};

/// @copydoc Row::extract
template <> int64_t Row::extract(const size_t n);
/// @copydoc Row::extract
template <> double Row::extract(const size_t n);
/// @copydoc Row::extract
template <> const void *Row::extract(const size_t n);
/// @copydoc Row::extract
template <> const unsigned char* Row::extract(const size_t n);
