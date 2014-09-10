#ifndef _GSQLITE_IFACES
#define _GSQLITE_IFACES

#include <string>

class Showable {
    public:
        virtual ~Showable(){};

        virtual std::string show() = 0;
};

class Conditionable {
    public:
        virtual ~Conditionable(){};

        virtual bool operator!() = 0;
        virtual operator void*() = 0;
};

#endif
