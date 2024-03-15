#include <iostream>
#include <cstring>
//created 1:00 14.03
//end work 4:40 14.03
//14:30 - 
class MemoryHelper
{
public:
    MemoryHelper();
    MemoryHelper(const MemoryHelper&);

    MemoryHelper& operator= (const MemoryHelper&);

    int size() const;
    int capacity() const;
    void *get_ptr() const;

    void* meminc(int type_sz);
    
    ~MemoryHelper();
protected:
    int sz; 
    int cap; 
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
    
    if (other.ptr != nullptr) {
        ptr = new char[bytes_cap];
        std::memcpy(ptr, other.ptr, bytes_sz);
    } else {
        ptr = nullptr;
    } 
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
    if (other.ptr != nullptr) {
        std::memcpy(ptr, other.ptr, bytes_sz);
    }
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
        char *temp = new char[cap * type_sz];

        if (ptr != nullptr) {
            std::memcpy(temp, ptr, sz * type_sz);
        }

        delete[] ptr;
        ptr = temp;
    }

    sz++;

    bytes_sz = sz * type_sz;
    bytes_cap = cap * type_sz;

    return static_cast<void *>(ptr);

}


class IntVector: private MemoryHelper
{
public:
    using MemoryHelper::size;
    int& operator[](int);
    const int& operator[](int) const;
    void insert(int);

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


class DoubleVector: private MemoryHelper
{
public:
    using MemoryHelper::size;
    double& operator[](int);
    const double& operator[](int) const;
    void insert(double);
};

void 
DoubleVector::insert(double num) {
    double *dptr = static_cast<double*>(meminc(sizeof(double)));
    new (&(dptr[sz - 1])) double(num);
}

double& 
DoubleVector::operator[](int pos) {
    return (static_cast<double*>(get_ptr()))[pos];
}

const double& 
DoubleVector::operator[](int pos) const {
    return (static_cast<double*>(get_ptr()))[pos];
}


class IntVectorVector: private MemoryHelper
{
public:
    IntVectorVector() = default;
    IntVectorVector(const IntVectorVector&);
    IntVectorVector operator=(const IntVectorVector&); 

    using MemoryHelper::size;
    IntVector& operator[](int);
    const IntVector& operator[](int) const;
    void insert(const IntVector&);
    ~IntVectorVector();
};

IntVectorVector::IntVectorVector(const IntVectorVector& v):
    MemoryHelper(v)
{
    IntVector* this_ptr = (static_cast<IntVector*>(get_ptr()));
    IntVector* v_ptr = (static_cast<IntVector*>(v.get_ptr()));

    IntVector temp;

    for (int i = 0; i < v.size(); i++) {
        this_ptr = IntVector(*v_ptr++);
        std::cout << "gang ";
    }
    std::cout << std::endl;
}

IntVectorVector IntVectorVector::operator=(const IntVectorVector& v) {
    if (this == &v) {
        return *this;
    }


    return *this;
}

void 
IntVectorVector::insert(const IntVector& vec) {
    IntVector *vptr = static_cast<IntVector*>(meminc(sizeof(IntVector)));
    new (&(vptr[sz - 1])) IntVector(vec);
}

IntVector& 
IntVectorVector::operator[](int pos) {
    return (static_cast<IntVector*>(get_ptr()))[pos];
}

const IntVector& 
IntVectorVector::operator[](int pos) const {
    return (static_cast<IntVector*>(get_ptr()))[pos];
}

IntVectorVector::~IntVectorVector() {
    IntVector *vptr = static_cast<IntVector*>(get_ptr());
    for (int i = 0; i < this->size(); i++) {
        vptr[i].~IntVector();
    }
}


//#ifdef _main
int
main(){
    IntVector v;
    v.insert(10);
    v.insert(20);
    IntVectorVector m;
    m.insert(v);
    m[0][0] = 30;
    m.insert(IntVector());
    m.insert(IntVector());
    // 10 20
    std::cout << v[0] << ' ' << v[1] << std::endl;
    // 30 20
    std::cout << m[0][0] << ' ' << m[0][1] << std::endl;
    // 2 3 0
    std::cout << v.size() << ' ' << m.size() << ' ' << m[1].size() << std::endl;

    std::cout << "NEW TESTS:" << std::endl;

/*
    IntVector v1(v);
    for (int i = 0; i < v1.size(); ++i) {
        std::cout << v1[i] << ' ';
    }
    std::cout << std::endl;
*/

    IntVectorVector m1(m);
    if (&m[0] == &m1[0]) {
        std::cout << "bad..." << std::endl;
    }
    for (int i = 0; i < m1.size(); ++i) {
        for (int j = 0; j < m1[i].size(); ++j) {
            std::cout << m1[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    m1[1].insert(123);
    
    for (int i = 0; i < m1.size(); ++i) {
        for (int j = 0; j < m1[i].size(); ++j) {
            std::cout << m1[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[i].size(); ++j) {
            std::cout << m[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}
//#endif