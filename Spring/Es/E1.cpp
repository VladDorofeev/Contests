#include <iostream>
#include <cstring>
//created 1:00 14.03
class MemoryHelper
{
public:
    MemoryHelper();
    ~MemoryHelper();

    int size() const;
    int capacity() const;
protected:
    void* realloc(int type_sz);
    int sz = 0; 
    int cap = 0; 
private:
    char *ptr = nullptr;
    enum { START_SZ = 2 };
};

MemoryHelper::MemoryHelper(): sz(0), cap(0), ptr(nullptr) {};
MemoryHelper::~MemoryHelper() {
    delete[] ptr;
}

int MemoryHelper::size() const {return sz;};
int MemoryHelper::capacity() const {return cap;};

void* 
MemoryHelper::realloc(int type_sz) {
    if (sz == cap) {
        cap = cap == 0 ? START_SZ : cap * 4;
        char *temp = new char[cap * type_sz];

        if (ptr != nullptr) {
            std::memcpy(temp, ptr, sz * type_sz);
        }

        delete[] ptr;
        ptr = temp;
    }

    sz++;

    return static_cast<void *>(ptr);

}


class IntVector: public MemoryHelper
{
public:
    IntVector();
    IntVector(const IntVector&);

    IntVector& operator=(const IntVector&);
    
    int& operator[](int);
    const int& operator[](int) const;

    void insert(int);
    int* get_ptr() {return ptr;};
private:
    int *ptr;
};
IntVector::IntVector(): ptr(nullptr) {};
void 
IntVector::insert(int num) {
    ptr = static_cast<int*>(realloc(sizeof(int)));
    new (&(ptr[sz - 1])) int(num);
}



int
main(){
    IntVector vec;
    vec.insert(1);
    vec.insert(2);
    vec.insert(3);
    vec.insert(4);
    vec.insert(5);
    std::cout << (vec.get_ptr())[0] << std::endl;
    std::cout << (vec.get_ptr())[1] << std::endl;
    std::cout << (vec.get_ptr())[2] << std::endl;
    std::cout << (vec.get_ptr())[3] << std::endl;
    std::cout << (vec.get_ptr())[4] << std::endl;
    return 0;
}