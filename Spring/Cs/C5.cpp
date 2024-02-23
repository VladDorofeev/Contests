#include <iostream>

class A
{
public:
    void get_0() const;
    static int x ;
    A();

};


void
A::get_0() const {
    std::cout << "I AM THE BEST STUDENT!" << std::endl;
}

A::A() {
    x = 123;
}