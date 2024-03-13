#include <iostream>

class A
{
public:
    A();

    void set_color(int num);
    int get_color() const;
private:
    int color;
};
A::A(): color(0) {};

void 
A::set_color(int num) {
    color = num;
}

int 
A::get_color() const {
    return color;
}

std::ostream& 
operator<< (std::ostream &os, const A &var) {
    os << "color = " << var.get_color();   
    return os;
}



class UniquePtr
{
public:
    UniquePtr();

    UniquePtr(UniquePtr &uptr);

    UniquePtr(A *a);

    UniquePtr& operator= (UniquePtr &uptr);

    A* operator-> ();
    const A* operator-> () const;
    
    A& operator* ();
    const A& operator* () const;


    ~UniquePtr();

private:
    A *ptr;
};

UniquePtr::UniquePtr() {
    this->ptr = nullptr;
}
UniquePtr::UniquePtr(UniquePtr &uptr) {
    ptr = uptr.ptr;
    uptr.ptr = nullptr;
}
UniquePtr::UniquePtr(A *a): ptr(a) {}

UniquePtr& 
UniquePtr::operator= (UniquePtr &uptr) {
    if (this == &uptr) {
        return *this;
    }

    delete ptr;
    ptr = uptr.ptr;
    uptr.ptr = nullptr;
    return *this;
}

A*
UniquePtr::operator-> () {
    return ptr;
}

const A* 
UniquePtr::operator-> () const {
    return ptr;
}


UniquePtr::~UniquePtr() {
    delete ptr;
}

A& 
UniquePtr::operator* () {
    return *ptr;
}

const A& 
UniquePtr::operator* () const {
    return *ptr;
}

#ifdef _main
int main()
{
    UniquePtr p = new A; // get ownership
    p->set_color(42);
    std::cout << *p << std::endl; // color = 42

    UniquePtr const& q = p; // ownership is not moved
    p->set_color(111);
    std::cout << q->get_color() << std::endl; // 111
    std::cout << *q << std::endl; // color = 111

    UniquePtr s = new A;
    s->set_color(31);
    std::cout << (*s).get_color() << std::endl; // 31

    s = p; // ownership is moved
    std::cout << s->get_color() << std::endl; // 111
    UniquePtr t = s; // ownership is moved
    t = t = t;
    std::cout << t->get_color() << std::endl; // 111
} // no memory leaks
#endif