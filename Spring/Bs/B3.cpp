#include <iostream>

using std::cout;
using std::endl;

class A
{
public:
    A();
    A(const A &);
    A(double);
    A(float, unsigned short);
    ~A();
    void m();
};

#if 0
int
main() {
    A a;
    cout << "start m method" << endl;
    a.m();
    cout << "end m method" << endl;
    return 0;
}
#endif

A::A() {
    cout << "1" << endl;
}

A::A(const A &a) {
    cout << "2" << endl;
}

A::A(double b) {
    cout << "3" << endl;
}

A::A(float a, unsigned short b) {
    cout << "4" << endl;
}

A::~A() {
    cout << "5" << endl;
}

void 
A::m() {
    //1 5 3 2 5 5 2 4 5 5
    A *a = new A();//1
    delete a;//5

    A *b = new A(1.1);//3
    A *c = new A(*b);//2
    delete c;//5
    delete b;//5

    A *d = new A(*b);//2
    A *e = new A(123.2, 1);//4
    delete d;//5
    delete e;//5
}