#include <iostream>


class A;

class UniquePtr {
private:
    char* ptr;
    int size;
    A* obj;
    UniquePtr(const int&);
public:
    UniquePtr();
    UniquePtr(A *);
    UniquePtr(UniquePtr&&);
    UniquePtr(UniquePtr&);
    ~UniquePtr();
    static UniquePtr make(const int&);
    char& get(const int&);
    const char& get(const int&) const;
    A* operator->();
    const A* operator->() const;
    A& operator*();
    const A& operator*() const;
    UniquePtr& operator=(UniquePtr&);
    UniquePtr& operator=(UniquePtr&&);
};

class A {
private:
    int color;
public:
    A();
    void set_color(int);
    int get_color() const;
    friend std::ostream& operator << (std::ostream &, const A&);
};

UniquePtr::UniquePtr(const int& s) {
        size = s;
        if (size) {
            ptr = new char [size];
        } else {
            ptr = nullptr;
        }
        obj = nullptr;
}

UniquePtr::UniquePtr() {
    size = 0;
    obj = nullptr;
    ptr = nullptr;
}

UniquePtr::UniquePtr(UniquePtr& other) {
    size = other.size;
    ptr = other.ptr;
    obj = other.obj;
    other.ptr = nullptr;
    other.obj = nullptr;
    other.size = 0;
}

UniquePtr::UniquePtr(UniquePtr&& other) {
    size = other.size;
    ptr = other.ptr;
    obj = other.obj;
    other.ptr = nullptr;
    other.obj = nullptr;
    other.size = 0;
}

UniquePtr::~UniquePtr() {
    if (ptr) {
        delete[] ptr;
    }
    delete obj;
 }

UniquePtr UniquePtr::make(const int& s) {
    return UniquePtr(s);
}

char& UniquePtr::get(const int& pos) {
    return ptr[pos];
}

const char& UniquePtr::get(const int& pos) const {
    return ptr[pos];
}

A::A() {
    color = 0;
}

UniquePtr::UniquePtr(A* ash) {
    obj = ash;
    size = 0;
    ptr = nullptr;
}

const A* UniquePtr::operator->() const {
    return obj;
}

A* UniquePtr::operator->() {
    return obj;
}

A& UniquePtr::operator*() {
    return *obj;
}

const A& UniquePtr::operator*() const {
    return *obj;
}

void A::set_color(int col) {
    color = col;
}

int A::get_color() const {
    return color;
}

std::ostream& operator << (std::ostream &out, const A& ash) {
    return out << "color = " <<  ash.get_color();
}

UniquePtr& UniquePtr::operator=(UniquePtr& other) {
    if (this != &other) {
        delete [] ptr;
        delete obj;
        size = other.size;
        ptr = other.ptr;
        obj = other.obj;
        other.ptr = nullptr;
        other.obj = nullptr;
        other.size = 0;
    }
    return *this;
}

UniquePtr& UniquePtr::operator=(UniquePtr&& other) {
    std::cout << "rvalue =\n";
    delete [] ptr;
    delete obj;
    size = other.size;
    ptr = other.ptr;
    obj = other.obj;
    other.ptr = nullptr;
    other.obj = nullptr;
    other.size = 0;
    return *this;
}

int
main() {
    /*UniquePtr p = new A; // get ownership
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
    return 0;*/
    UniquePtr p = new A;
    p = UniquePtr(new A);
}