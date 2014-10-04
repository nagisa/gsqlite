
#include<iostream>
#include<ostream>
#include<fstream>

#define SHOULD(description, cond) {{\
    if(!(cond)){\
        std::cerr << "FAILED SHOULD: " << description << "\n";\
        return 1;\
    }\
}}

#define SHOULD_THROW(desc, exc, code) {{\
    try{\
        code;\
        std::cerr << "FAILED SHOULD_THROW: " << desc << "\n";\
        return 1;\
    } catch (exc &e) {\
    } catch (...){\
        std::cerr << "FAILED SHOULD_THROW: " << desc << ": wrong exception" \
               << "\n";\
        return 1;\
    }\
}}
