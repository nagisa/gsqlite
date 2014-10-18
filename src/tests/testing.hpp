#include<iostream>
#include<ostream>
#include<fstream>
#include<cstdlib>

#define SHOULD(description, cond) {{\
    if(!(cond)){\
        std::clog << "FAILED SHOULD: " << description << "\n";\
        std::exit(EXIT_FAILURE);\
    }\
}}

#define SHOULD_THROW(desc, exc, code) {{\
    try{\
        code;\
        std::cerr << "FAILED SHOULD_THROW: " << desc << "\n";\
        std::exit(EXIT_FAILURE);\
    } catch (exc &e) {\
    } catch (...){\
        std::clog << "FAILED SHOULD_THROW: " << desc << ": wrong exception" \
               << "\n";\
        std::exit(EXIT_FAILURE);\
    }\
}}
