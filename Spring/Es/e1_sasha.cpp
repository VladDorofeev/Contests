#include <iostream>
#include <cstring>

class VoidVector {
public:
    VoidVector();
    VoidVector(const VoidVector&);
    VoidVector& operator=(const VoidVector&);
    ~VoidVector();
protected:
    void* get_pt(int, size_t) const;
    void insert(const void*, size_t);
    int size() const;
    void arr_clear();
private:
    int cnt_elem;
    size_t cnt_bytes;
    size_t capacity_bytes;
    char* array;
};

VoidVector::VoidVector()
    : cnt_elem(0)
    ,cnt_bytes(0)
    , capacity_bytes(0)
    , array(nullptr)
{
}
VoidVector::VoidVector(const VoidVector& other)
    : cnt_elem(other.cnt_elem)
    , cnt_bytes(other.cnt_bytes)
    , capacity_bytes(other.capacity_bytes) 
    ,array(nullptr)
{
    if (other.array) {
        array = new char[capacity_bytes];
        memcpy(array, other.array, cnt_bytes);
    }
}
VoidVector::~VoidVector() {
    delete[] array;
    cnt_elem = 0;
    cnt_bytes = 0;
    capacity_bytes = 0;
}
VoidVector& VoidVector::operator=(const VoidVector& other)
{
    if (array != other.array) {
        cnt_elem = other.cnt_elem;	
        cnt_bytes = other.cnt_bytes;
        capacity_bytes = other.capacity_bytes;
        delete[] array;
        array = new char[capacity_bytes];
        memcpy(array, other.array, cnt_bytes);
    }
    return *this;
}

int VoidVector::size() const {
    return cnt_elem;
}
void VoidVector::insert(const void* elem, size_t elem_size) {
    if (capacity_bytes == cnt_bytes) {
        char* new_arr = nullptr;
        if (capacity_bytes == 0) {
            capacity_bytes = elem_size*100;
        } else {
            capacity_bytes = 2 * capacity_bytes;
        }
        new_arr = new char [capacity_bytes];
        if (array) {
             memcpy(new_arr, array, cnt_bytes);
        }	
        delete[] array;
        array = new_arr;
    }
    memcpy(array + cnt_bytes, elem, elem_size);
    cnt_bytes += elem_size;
    ++cnt_elem;
}

void VoidVector::arr_clear() {
    delete[] array;
    array = nullptr;
    cnt_elem = 0;
    cnt_bytes = 0;
    capacity_bytes = 0;
}

void* VoidVector::get_pt(int index, size_t elem_size) const {
    return array + index*elem_size;
}

class IntVector: private VoidVector {
public:
    IntVector();
    IntVector(const IntVector&);
    using VoidVector::operator=;
    using VoidVector::size;
    void insert(int);
    int& operator[](int index);
    const int& operator[](int index) const;
};

IntVector::IntVector()
    : VoidVector() 
{
}

IntVector::IntVector(const IntVector& other) 
    : VoidVector(other) 
{
}

void IntVector::insert(int num) {
    VoidVector::insert(&num, sizeof(int));
}

int& IntVector::operator[](int index) {
    return *(static_cast<int*>(get_pt(index, sizeof(int))));
}
const int& IntVector::operator[](int index) const {
    return *(static_cast<int*>(get_pt(index, sizeof(int))));
}

class DoubleVector: private VoidVector {
public:
    DoubleVector();
    DoubleVector(const DoubleVector&);
    using VoidVector::operator=;
    using VoidVector::size;
    void insert(double);
    double& operator[](int index);
    const double& operator[](int index) const;
};

DoubleVector::DoubleVector()
    : VoidVector() 
{
}
DoubleVector::DoubleVector(const DoubleVector& other) 
    : VoidVector(other) 
{
}

void DoubleVector::insert(double num) {
    VoidVector::insert(&num, sizeof(double));
}

double& DoubleVector::operator[](int index) {
    return *(static_cast<double*>(get_pt(index, sizeof(double))));
}
const double& DoubleVector::operator[](int index) const {
    return *(static_cast<double*>(get_pt(index, sizeof(double))));
}

class IntVectorVector: private VoidVector {
public:
    IntVectorVector();
    IntVectorVector(const IntVectorVector&);
    ~IntVectorVector();
    IntVectorVector& operator=(const IntVectorVector&);
    using VoidVector::size;
    void insert(IntVector);
    IntVector& operator[](int index);
    const IntVector& operator[](int index) const;
};

IntVectorVector::IntVectorVector()
    : VoidVector() 
{
}

IntVectorVector::IntVectorVector(const IntVectorVector& other) 
    : VoidVector()
{
    IntVector* vector_ex = nullptr;
    for(int i = 0; i < other.size(); ++i) {
        vector_ex = static_cast<IntVector*>(operator new[] (sizeof(IntVector)));
        new (vector_ex) IntVector(other[i]);
        VoidVector::insert(vector_ex, sizeof(IntVector));
        operator delete[] (vector_ex);
    }
}
IntVectorVector::~IntVectorVector() {
    for(int i = 0; i < size(); ++i) {
        (*(static_cast<IntVector*>(get_pt(i, sizeof(IntVector))))).~IntVector();
    }
}
IntVectorVector& IntVectorVector::operator=(const IntVectorVector& other) {
    if (get_pt(0, 0) != other.get_pt(0, 0)) {
        for(int i = 0; i < size(); ++i) {
            (*(static_cast<IntVector*>(this->get_pt(i, sizeof(IntVector))))).~IntVector();
        }
        arr_clear();
        IntVector* vector_ex = nullptr;
        for(int i = 0; i < other.size(); ++i) {
            vector_ex = static_cast<IntVector*>(operator new[] (sizeof(IntVector)));
            new (vector_ex) IntVector(other[i]);
            VoidVector::insert(vector_ex, sizeof(IntVector));
            operator delete[] (vector_ex);
        }
    }
    return *this;
}
void IntVectorVector::insert(IntVector int_vector) {
    IntVector* vector_ex = static_cast<IntVector*>(operator new[] (sizeof(IntVector)));
    new (vector_ex) IntVector(int_vector);
    VoidVector::insert(vector_ex, sizeof(IntVector));
    operator delete[] (vector_ex);
}

IntVector& IntVectorVector::operator[](int index) {
    return *(static_cast<IntVector*>(get_pt(index, sizeof(IntVector))));
}
const IntVector& IntVectorVector::operator[](int index) const {
    return *(static_cast<IntVector*>(get_pt(index, sizeof(IntVector))));
}


/*int
main() {
IntVector v;
v.insert(10);
v.insert(20);
IntVector v1;
v1 = v;
std::cout << v1.size() << std::endl;
IntVectorVector m;
m.insert(v);
m[0][0] = 30;
m.insert(IntVector());
m.insert(IntVector());
m = m = m;
// 10 20
std::cout << v[0] << ' ' << v[1] << std::endl;
// 30 20
std::cout << m[0][0] << ' ' << m[0][1] << std::endl;
// 2 3 0
std::cout << v.size() << ' ' << m.size() << m[1].size() << std::endl;
v = v = v;
std::cout << v[0] << ' ' << v[1] << std::endl;
}*/
