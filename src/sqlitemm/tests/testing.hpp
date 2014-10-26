#include<iostream>
#include<ostream>
#include<fstream>
#include<cstdlib>

#define SHOULD(description, cond) {{\
    if(!(cond)){\
        using namespace std;\
        clog << "FAILED SHOULD: " << description << "\n";\
        exit(EXIT_FAILURE);\
    }\
}}

#define SHOULD_THROW(desc, exc, code) {{\
    try{\
        code;\
        using namespace std;\
        clog << "FAILED SHOULD_THROW: " << desc << "\n";\
        exit(EXIT_FAILURE);\
    } catch (exc &e) {\
    } catch (...){\
        using namespace std;\
        clog << "FAILED SHOULD_THROW: " << desc << ": wrong exception" \
               << "\n";\
        exit(EXIT_FAILURE);\
    }\
}}
