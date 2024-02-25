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
    static UniquePtr make(int sz);

private:
    char *ptr;
    int sz;
    UniquePtr(int _sz);
};

using namespace std;

UniquePtr::UniquePtr() {
    this->ptr = nullptr;
    this->sz = 0;
}
UniquePtr::UniquePtr(UniquePtr &uptr) {
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
}
UniquePtr::UniquePtr(UniquePtr &&uptr) {
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
}
UniquePtr::UniquePtr(int _sz) {
    ptr = new char[_sz];
    sz = _sz;
}

const UniquePtr& 
UniquePtr::operator= (UniquePtr &uptr) {
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
    return *this;
}
const UniquePtr& 
UniquePtr::operator= (UniquePtr &&uptr) {
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
    return *this;
}

UniquePtr::~UniquePtr() {
    delete[] ptr;
}

char&
UniquePtr::get(int pos) const {
    return ptr[pos];
}

UniquePtr
UniquePtr::make(int _sz) {
    return std::move(UniquePtr(_sz));
}

#ifdef _main
int
main() {
    UniquePtr uptr = UniquePtr::make(10000);
    uptr.get(0) = 'a';
    cout << endl;

    UniquePtr uptr2;
    uptr2 = uptr;
    cout << uptr2.get(0) << endl;
    cout << endl;

    const UniquePtr uptr3 = UniquePtr::make(10000);
    cout << endl;


    return 0;
}
#endif