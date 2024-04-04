#include <iostream>
#include <cstring>
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

    enum { START_SZ = 1000 };
};

MemoryHelper::MemoryHelper(): 
    sz(0), cap(0), ptr(nullptr), bytes_cap(0), bytes_sz(0) 
{};

MemoryHelper::MemoryHelper(const MemoryHelper& other) 
    : sz(other.sz)
    , cap(other.cap)
    , bytes_cap(other.bytes_cap)
    , bytes_sz(other.bytes_sz)
{
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
    if (other.ptr != nullptr) {
        ptr = new char[bytes_cap];
        std::memcpy(ptr, other.ptr, bytes_sz);
    } else {
        ptr = nullptr;
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

template <typename T>
class Vector: private MemoryHelper
{
public:
    Vector() = default;
    Vector(const Vector&);
    Vector& operator=(const Vector&); 

    using MemoryHelper::size;
    T& operator[](int);
    const T& operator[](int) const;
    void insert(const T&);
    ~Vector();
};

template <typename T>
Vector<T>::Vector(const Vector<T>& v):
    MemoryHelper(v)
{
    T* this_ptr = (static_cast<T*>(get_ptr()));
    T* v_ptr = (static_cast<T*>(v.get_ptr()));

    for (int i = 0; i < v.size(); i++) {
        new (&this_ptr[i]) T(v_ptr[i]);
    }
}

template <typename T>
Vector<T>&
Vector<T>::operator=(const Vector<T>& v) {
    //If we have same object
    if (this == &v) {
        return *this;
    }

    //Clear old T in this
    T *vptr = static_cast<T*>(get_ptr());
    for (int i = 0; i < this->size(); i++) {
        vptr[i].~T();
    }

    //Call base operator =
    MemoryHelper::operator=(v);

    //Create new T (copy from Vector v)
    T* this_ptr = (static_cast<T*>(get_ptr()));
    T* v_ptr = (static_cast<T*>(v.get_ptr()));
    
    for (int i = 0; i < v.size(); i++) {
        new (&this_ptr[i]) T(v_ptr[i]);
    }

    return *this;
}

template <typename T>
void 
Vector<T>::insert(const T& vec) {
    T *vptr = static_cast<T*>(meminc(sizeof(T)));
    new (&(vptr[sz - 1])) T(vec);
}

template <typename T>
T& 
Vector<T>::operator[](int pos) {
    return (static_cast<T*>(get_ptr()))[pos];
}

template <typename T>
const T& 
Vector<T>::operator[](int pos) const {
    return (static_cast<T*>(get_ptr()))[pos];
}

template <typename T>
Vector<T>::~Vector() {
    T *vptr = static_cast<T*>(get_ptr());
    for (int i = 0; i < this->size(); i++) {
        vptr[i].~T();
    }
}


#ifdef _main
int
main(){
    // Vector<int> a;
    // a.insert(1);
    // a.insert(2);
    // a.insert(3);
    // a.insert(4);

    // Vector<Vector<int>> v;
    // v.insert(a);
    // v.insert(a);
    // v.insert(a);

    // for (int i = 0; i < v.size(); ++i) {
    //     for (int j = 0; j < v[i].size(); ++j) {
    //         std::cout << v[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    // Vector<Vector<int>> q(v);
    // for (int i = 0; i < q.size(); ++i) {
    //     for (int j = 0; j < q[i].size(); ++j) {
    //         std::cout << q[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    // q = Vector<Vector<int>>();
    // q = v;
    // for (int i = 0; i < q.size(); ++i) {
    //     for (int j = 0; j < q[i].size(); ++j) {
    //         std::cout << q[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
    // q = v;
    // for (int i = 0; i < q.size(); ++i) {
    //     for (int j = 0; j < q[i].size(); ++j) {
    //         std::cout << q[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    // q=q=q=q=q;
    // q = Vector(v);
    // for (int i = 0; i < q.size(); ++i) {
    //     for (int j = 0; j < q[i].size(); ++j) {
    //         std::cout << q[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
    Vector<int> v;
    v.insert(10);
    Vector<Vector<double>> v2;
    v2.insert({});
    v2[0].insert(.5);
    return 0;
}
#endif