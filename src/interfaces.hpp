#pragma once

#include <string>

/// Interface class which provides a method to retrieve textual object
/// representation.
class Showable {
    public:
        virtual ~Showable(){};

        virtual std::string show() = 0;
};


/// Interface class which provides methods to check whether object is truthy.
class Conditionable {
    public:
        virtual ~Conditionable(){};

        virtual bool operator!() = 0;
        virtual operator void*() = 0;
};
