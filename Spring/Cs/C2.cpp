#include <iostream>


class UniquePtr
{
public:
    UniquePtr();

    UniquePtr(UniquePtr &uptr);
    UniquePtr(UniquePtr &&uptr);

    const UniquePtr& operator= (UniquePtr &uptr);
    const UniquePtr& operator= (UniquePtr &&uptr);

    ~UniquePtr();

    char &get(int pos) const;
    friend UniquePtr make(int sz);

private:
    char *ptr;
    int sz;
    UniquePtr(int _sz);
};

using namespace std;

UniquePtr::UniquePtr() {
    cout << "def ctor" << endl;
    this->ptr = nullptr;
    this->sz = 0;
}
UniquePtr::UniquePtr(UniquePtr &uptr) {
    cout << "copy ctor" << endl;
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
}
UniquePtr::UniquePtr(UniquePtr &&uptr) {
    cout << "move ctor" << endl;
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
}
UniquePtr::UniquePtr(int _sz) {
    cout << "params ctor" << endl;
    ptr = new char[_sz];
    sz = _sz;
}

const UniquePtr& 
UniquePtr::operator= (UniquePtr &uptr) {
    cout << "oper = &" << endl;
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
    return *this;
}
const UniquePtr& 
UniquePtr::operator= (UniquePtr &&uptr) {
    cout << "oper = &&" << endl;
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
    return *this;
}

UniquePtr::~UniquePtr() {
    cout << "destructor" << endl;    
    delete[] ptr;
}

char&
UniquePtr::get(int pos) const {
    cout << "get worked" << endl;
    return ptr[pos];
}

UniquePtr
make(int _sz) {
    cout << "start make" << endl;
    cout << "end make" << endl;
    return std::move(UniquePtr(_sz));
}

#ifdef _main
int
main() {
    UniquePtr uptr = make(10000);
    uptr.get(0) = 'a';
    cout << endl;

    UniquePtr uptr2;
    uptr2 = uptr;
    cout << uptr2.get(0) << endl;
    cout << endl;

    const UniquePtr uptr3 = make(10000);
    cout << endl;


    return 0;
}
#endif