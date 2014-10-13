#pragma once

#include <string>

/// Objects that have a meaningful textual representation.
class Showable {
    public:
        virtual ~Showable(){};

        /// Retrieve a meaningful text representation of an object.
        ///
        virtual std::string show() = 0;
};


/// Objects that may be truthy of falsey.
class Conditionable {
    public:
        virtual ~Conditionable(){};

        /// Negate the object.
        ///
        /// Must return boolean value oposite from logic value object
        /// represents.
        virtual bool operator!() = 0;
        /// Return a reference basic value (void pointer) which represents
        /// has the same logic value as an object on which this cast is called.
        ///
        /// If the object is falsy, nullptr must be returned and
        /// a non-null pointer (e.g. pointer to object this method is called
        /// on) otherwise.
        virtual operator void*() = 0;
};
