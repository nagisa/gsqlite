#pragma once

#include <vector>
#include <memory>

#include "sqlite_common.hpp"
#include "value.hpp"

class Row : public Showable {
    public:
        Row(std::vector<std::unique_ptr<Value>>);
        ~Row();

        /// Get amount of columns in the row.
        size_t columns();

        /// Get value at column n (0-indexed)
        /// May return nullptr indicating that column is NULL.
        /// Will throw std::logic_error if the row is invalid.
        /// Value belongs to Row and should not be deleted.
        /// Warning: bounds are not checked, indexing over bounds is undefined.
        Value * operator[](size_t n);

        /// Extract usable value at column n (0-indexed)
        /// Implemented template specialisations are int64_t (for IntValue),
        /// double (FloatValue), const unsigned char* (TextValue) and
        /// const void * (BlobValue). Will throw if:
        /// 1) (std::bad_cast) requested value type does not match type stored
        ///    in db. i.e. you call exract<double> while we have IntValue.
        /// 2) (SQLiteException) column n is NULL;
        /// 3) (std::logic_error) row is invalid.
        /// Warning: bounds are not checked, indexing over bounds is undefined.
        template<typename T> T extract(size_t n);

        void invalidate();

        std::string show() override;
    private:
        std::vector<std::unique_ptr<Value>> values;

        bool invalid_ = false;
        void ensure_valid();
};
