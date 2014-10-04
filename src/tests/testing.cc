#include "testing.hpp"
#include <cassert>

int should1(){
    SHOULD("should should do nothing if true", true);
    return 0;
}

int should2(){
    SHOULD("should should return 1 if false", false);
    return 0;
}

int should_throw1(){
    SHOULD_THROW("should_throw should do nothing if correct exception",
                 std::exception,
                 { throw std::exception(); });
    return 0;
}

int should_throw2(){
    SHOULD_THROW("should_throw should return 1 if incorrect exception",
                 std::exception,
                 { throw 1; });
    return 0;
}

int should_throw3(){
    SHOULD_THROW("should_throw should return 1 if no exception",
                 std::exception,
                 { ; });
    return 0;
}

int main(void){
    assert(should1() == 0);
    assert(should2() != 0);
    assert(should_throw1() == 0);
    assert(should_throw2() != 0);
    assert(should_throw3() != 0);
    return 0;
}
