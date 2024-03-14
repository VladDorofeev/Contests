#include <iostream>
#include <cstring>
//created 1:00 14.03
class MemoryHelper
{
public:
    MemoryHelper();
    MemoryHelper(const MemoryHelper&);

    MemoryHelper& operator= (const MemoryHelper&);

    int size() const;
    int capacity() const;
    
    void *get_ptr() const;
protected:
    ~MemoryHelper();
    void* meminc(int type_sz);
    int sz; 
    int cap; 
private:
    char *ptr;
    int bytes_cap;
    int bytes_sz;

    enum { START_SZ = 2 };
};

MemoryHelper::MemoryHelper(): 
    sz(0), cap(0), ptr(nullptr), bytes_cap(0), bytes_sz(0) 
{};
MemoryHelper::MemoryHelper(const MemoryHelper& other) {
    sz = other.sz;
    cap = other.cap;
    bytes_cap = other.bytes_cap;
    bytes_sz = other.bytes_sz;

    ptr = new char[bytes_cap];
    std::memcpy(ptr, other.ptr, bytes_sz);
}

MemoryHelper& 
MemoryHelper::operator= (const MemoryHelper& other) {
    if (this == &other) {
        return *this;
    }
    sz = other.sz;
    cap = other.cap;
    bytes_cap = other.bytes_cap;
    bytes_sz = other.bytes_sz;

    delete[] ptr;
    ptr = new char[bytes_cap];
    std::memcpy(ptr, other.ptr, bytes_sz);
    return *this;
}

MemoryHelper::~MemoryHelper() {
    delete[] ptr;
}

int MemoryHelper::size() const {return sz;};
int MemoryHelper::capacity() const {return cap;};
void *
MemoryHelper::get_ptr() const {return static_cast<void *>(ptr);};


void* 
MemoryHelper::meminc(int type_sz) {
    if (sz == cap) {
        cap = cap == 0 ? START_SZ : cap * 2;
        bytes_cap = cap * type_sz;
        char *temp = new char[cap * type_sz];

        if (ptr != nullptr) {
            std::memcpy(temp, ptr, sz * type_sz);
        }

        delete[] ptr;
        ptr = temp;
    }

    sz++;
    bytes_sz = sz * type_sz;

    return static_cast<void *>(ptr);

}


class IntVector: public MemoryHelper
{
public:
    IntVector() = default;
    IntVector(const IntVector&) = default;

    IntVector& operator=(const IntVector&) = default;
    
    int& operator[](int);
    const int& operator[](int) const;

    void insert(int);
private:
};

void 
IntVector::insert(int num) {
    int *iptr = static_cast<int*>(meminc(sizeof(int)));
    new (&(iptr[sz - 1])) int(num);
}

int& 
IntVector::operator[](int pos) {
    return (static_cast<int*>(get_ptr()))[pos];
}

const int& 
IntVector::operator[](int pos) const {
    return (static_cast<int*>(get_ptr()))[pos];
}



int
main(){
    IntVector vec;
    vec.insert(1);
    vec.insert(2);
    vec.insert(3);
    vec.insert(4);
    vec.insert(5);
    IntVector vec2;

    vec2 = vec;

    for (int i = 0; i < vec2.size(); i++) {
        std::cout << vec2[i] << std::endl;
    }
    return 0;
}