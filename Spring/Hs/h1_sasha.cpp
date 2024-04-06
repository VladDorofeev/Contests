#include <iostream>
#include <cstring>

class VoidVector {
public:
    VoidVector();
    VoidVector(const VoidVector&);
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


template<typename T>
class Vector: private VoidVector {
public:
    Vector();
    Vector(const Vector<T>&);
    ~Vector();

    Vector& operator=(const Vector<T>&);
    T& operator[](int index);
    const T& operator[](int index) const;

    void kill_elems() const;
    void insert(T);
};


//VoidVector impl
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


VoidVector::~VoidVector() 
{
    delete[] array;
    cnt_elem = 0;
    cnt_bytes = 0;
    capacity_bytes = 0;
}


int VoidVector::size() const 
{
    return cnt_elem;
}


void VoidVector::insert(const void* elem, size_t elem_size) 
{
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


void VoidVector::arr_clear()
{
    delete[] array;
    array = nullptr;
    cnt_elem = 0;
    cnt_bytes = 0;
    capacity_bytes = 0;
}


void* VoidVector::get_pt(int index, size_t elem_size) const 
{
    return array + index*elem_size;
}


//Vector impl


template<typename T>
Vector<T>::Vector()
    : VoidVector() 
{
}


template<typename T>
Vector<T>::Vector(const Vector<T>& other) 
    : VoidVector()
{
    for(int i = 0; i < other.size(); ++i) {
        insert(other[i]);
    }
}


template<typename T>
Vector<T>::~Vector() 
{
    kill_elems();
}


template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) 
{
    if (this != &other) {
        kill_elems();
        arr_clear();
        for(int i = 0; i < other.size(); ++i) {
            insert(other[i]);
        }
    }
    return *this;
}


template<typename T>
void Vector<T>::kill_elems() const {
    for(int i = 0; i < size(); ++i) {
        (*(static_cast<T*>(this->get_pt(i, sizeof(T))))).~T();
    }
}


template<typename T>
void Vector<T>::insert(T elem) 
{
    T* new_elem = static_cast<T*>(operator new[] (sizeof(T)));
    new (new_elem) T(elem);
    VoidVector::insert(new_elem, sizeof(T));
    operator delete[] (new_elem);
}


template<typename T>
T& Vector<T>::operator[](int index) 
{
    return *(static_cast<T*>(get_pt(index, sizeof(T))));
}


template<typename T>
const T& Vector<T>::operator[](int index) const 
{
    return *(static_cast<T*>(get_pt(index, sizeof(T))));
}


int main()
{
    Vector<int> v;
    v.insert(10);
    Vector<Vector<double>> v2;
    v2.insert({});
    v2[0].insert(.5);
    std::cout << v2[0][0] << std::endl;
}