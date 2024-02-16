#include <iostream>

using std::cout;
using std::endl;

class A
{
    int a;
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
    this->a = 0;
    cout << "1" << endl;
}

A::A(const A &b) {
    this->a = b.a;
    cout << "2" << endl;
}

A::A(double b) {
    this->a = static_cast<int> (b);
    cout << "3" << endl;
}

A::A(float f, unsigned short us) {
    this->a = static_cast<int> (us);
    cout << "4" << endl;
}

A::~A() {
    cout << "5" << endl;
}


void 
A::m() {
    //1 5 3 2 5 5 2 4 5 5
    A *f = new A();//1
    delete f;//5

    A *b = new A(2.1341341);//3
    A *c = new A(*b);//2
    delete c;//5
    delete b;//5

    A *d = new A(*this);//2
    A *e = new A(123.3,11);//4
    
    delete e;//5
    delete d;//5

}