#include <iostream>


class UniquePtr {
private:
    char* ptr;
    int size;
    //UniquePtr(const int&);
public:
UniquePtr(const int&);
    UniquePtr();
    UniquePtr(UniquePtr&&);
    UniquePtr(UniquePtr&);
    ~UniquePtr();
    static UniquePtr make(const int&);
    char& get(const int&);
    const char& get(const int&) const;
};

UniquePtr::UniquePtr(const int& s) {
    std::cout << "constructor conversion\n";
        size = s;
        if (size) {
            ptr = new char [size];
        } else {
            ptr = nullptr;
        }
}

UniquePtr::UniquePtr() {
    std::cout << "constructor default\n";
    size = 0;
    ptr = nullptr;
}

UniquePtr::UniquePtr(UniquePtr& other) {
    std::cout << "constructor copy&\n";
    size = other.size;
    ptr = other.ptr;
    other.ptr = nullptr;
    other.size = 0;
}

UniquePtr::UniquePtr(UniquePtr&& other) {
    std::cout << "constructor copy&&\n";
    size = other.size;
    ptr = other.ptr;
    other.ptr = nullptr;
    other.size = 0;
}

UniquePtr::~UniquePtr() {
    std::cout << "destructor\n";
    if (ptr) {
        delete[] ptr;
    }
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

int
main() {
   UniquePtr pt = UniquePtr::make(10);
    UniquePtr pt1 = UniquePtr(10);
    return 0;
}