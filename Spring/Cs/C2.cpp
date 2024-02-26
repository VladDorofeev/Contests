#include <iostream>

class UniquePtr
{
public:
    UniquePtr();

    UniquePtr(UniquePtr &uptr);
    UniquePtr(UniquePtr &&uptr);

    UniquePtr& operator= (UniquePtr &uptr);
    UniquePtr& operator= (UniquePtr &&uptr);

    ~UniquePtr();

    char &get(int pos);
    const char &get(int pos) const;

    static UniquePtr make(int sz);

private:
    char *ptr;
    int sz;
    UniquePtr(int _sz);
};

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

UniquePtr& 
UniquePtr::operator= (UniquePtr &uptr) {
    ptr = uptr.ptr;
    sz = uptr.sz;
    uptr.ptr = nullptr;
    return *this;
}
UniquePtr& 
UniquePtr::operator= (UniquePtr &&uptr) {
    this->ptr = uptr.ptr;
    this->sz = uptr.sz;
    uptr.ptr = nullptr;
    return *this;
}

UniquePtr::~UniquePtr() {
    delete[] ptr;
}

char&
UniquePtr::get(int pos) {
    return ptr[pos];
}

const char &
UniquePtr::get(int pos) const {
    return ptr[pos];
}


UniquePtr
UniquePtr::make(int _sz) {
    return UniquePtr(_sz);
}

#ifdef _main
int
main() {
    cout << "1" << endl;
    UniquePtr uptr1 = UniquePtr::make(10);
    uptr1.get(0) = 'a';
    cout << endl;

    cout << "2" << endl;
    UniquePtr uptr2;
    uptr2 = uptr1;
    cout << uptr2.get(0) << endl;
    cout << endl;


    cout << "3" << endl;
    UniquePtr uptr3;
    uptr3 = UniquePtr::make(10);
    uptr3.get(0) = 'a';
    cout << endl;


    cout << "4" << endl;
    UniquePtr uptr4 = uptr3;
    cout << uptr2.get(0) << endl;
    cout << endl;


    cout << "5" << endl;
    const UniquePtr uptr5(UniquePtr::make(100));
    uptr5.get(0) = 'c';
    cout << endl;


    return 0;
}
#endif