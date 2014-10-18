#pragma once

#include <vector>
#include <memory>
#include <glibmm.h>

#include "sqlite_common.hh"
#include "value.hpp"

/// @brief A single promise to retrieve a result row for the query.
class Row : public Showable, public Glib::Object {
    public:
        typedef std::vector<std::unique_ptr<Value>> values_t;
        typedef enum {
            PRISTINE = 0,
            RESOLVED,
            FAILED,
            DONE
        } state_t;
        typedef std::function<bool (state_t)> monitor_fn_t;

    private:
        values_t                            values;
        Glib::Property<std::exception_ptr>  _exception;
        Glib::Property<state_t>             _state;
    public:
        Glib::PropertyProxy_ReadOnly<state_t> state();
        Glib::PropertyProxy_ReadOnly<std::exception_ptr> exception();

        Row();
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
        Value * operator[](size_t n);

        /// Extract usable value at column n (0-indexed)
        ///
        /// Will throw if:
        /// 1. (std::bad_cast) requested value type does not match type stored
        ///    in db. i.e. you call exract<double> while we have IntValue.
        /// 2. (SQLiteException) column n is NULL;
        /// 3. (std::logic_error) row is invalid.
        ///
        /// Warning: bounds are not checked, indexing over bounds is undefined.
        template<typename T> T extract(size_t n);

        /// Resolve the Row with values_t.
        ///
        /// state::changed signal will be executed in thread with main loop
        /// regardless of calling thread.
        void resolve(values_t);

        /// Fail the row with exception.
        ///
        /// state::changed signal will be executed in thread with main loop
        /// regardless of calling thread.
        void fail(std::exception_ptr);

        /// Convenience function which will call the callback when
        /// state_t changes.
        ///
        /// Will monitor until callback returns false.
        void monitor(monitor_fn_t);

        std::string show() override;
    private:
        void ensure_resolved();
};

/// @copydoc Row::extract
template <> int64_t Row::extract(size_t n);
/// @copydoc Row::extract
template <> double Row::extract(size_t n);
/// @copydoc Row::extract
template <> const void *Row::extract(size_t n);
/// @copydoc Row::extract
template <> const unsigned char* Row::extract(size_t n);
