
#include<iostream>
#include<ostream>
#include<fstream>

template <class CharT, class Traits = std::char_traits<CharT>>
class Tee : virtual public std::basic_ostream<CharT, Traits> {
    std::ofstream out;

    public:
        Tee(){
            this->out.open("/tmp/gsqlite-test-out");
        };
        ~Tee(){
            this->out.flush();
            this->out.close();
        }
        template<typename T>
        Tee& operator<< (const T& t){
            std::cerr << t;
            this->out << t;
            return *this;
        }
};

static Tee<char> out;

#define SHOULD(description, cond) {{\
    if(!(cond)){\
        out << "FAILED SHOULD: " << description << "\n";\
        return 1;\
    }\
}}

#define SHOULD_THROW(desc, exc, code) {{\
    try{\
        code;\
        out << "FAILED SHOULD_THROW: " << desc << "\n";\
        return 1;\
    } catch (exc &e) {\
    } catch (...){\
        out << "FAILED SHOULD_THROW: " << desc << ": wrong exception" \
               << "\n";\
        return 1;\
    }\
}}
