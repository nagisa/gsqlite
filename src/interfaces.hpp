#pragma once

#include <string>

/// Interface class which provides a method to retrieve textual object
/// representation.
class Showable {
    public:
        virtual ~Showable(){};

        /// Generate a string representation of an object which implements
        /// the interface.
        virtual std::string show() = 0;
};


/// Interface class which provides methods to check whether object is truthy.
class Conditionable {
    public:
        virtual ~Conditionable(){};

        /// Negate the object which implements the operator.
        /// Must return boolean value oposite from logic value object
        /// represents.
        virtual bool operator!() = 0;
        /// Return a reference basic value (void pointer) which represents
        /// has the same logic value as an object on which this cast is called.
        /// If the object is falsy, nullptr must be returned and
        /// a non-null pointer (e.g. pointer to object this method is called
        /// on) otherwise.
        virtual operator void*() = 0;
};
