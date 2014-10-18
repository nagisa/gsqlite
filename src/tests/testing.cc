#include <cassert>
#include <sstream>
#include "testing.hpp"

int exit_calls = 0;

void check_exit(int errc){
    exit_calls += 1;
}

int should1(){
#define exit(code) check_exit(code)
    SHOULD("should should do nothing if true", true);
#undef exit
    return 0;
}

int should2(){
#define exit(code) check_exit(code)
    SHOULD("should should return 1 if false", false);
#undef exit
    return 0;
}

int should_throw1(){
#define exit(code) check_exit(code)
    SHOULD_THROW("should_throw should do nothing if correct exception",
                 std::exception,
                 { throw std::exception(); });
#undef exit
    return 0;
}

int should_throw2(){
#define exit(code) check_exit(code)
    SHOULD_THROW("should_throw should return 1 if incorrect exception",
                 std::exception,
                 { throw 1; });
#undef exit
    return 0;
}

int should_throw3(){
#define exit(code) check_exit(code)
    SHOULD_THROW("should_throw should return 1 if no exception",
                 std::exception,
                 { ; });
#undef exit
    return 0;
}

int main(void){
    assert(should1() == 0);
    assert(should_throw1() == 0);

    // These output garbage
    std::stringstream ss;
    std::clog.flush();
    auto *old = std::clog.rdbuf(ss.rdbuf());
    should2();
    should_throw2();
    should_throw3();
    std::clog.flush();
    std::clog.rdbuf(old);
    SHOULD("exit should be called 3 times", exit_calls == 3);
    return 0;
}
