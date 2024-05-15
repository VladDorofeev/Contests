#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>

template <class T>
class MyVector
{
public:
    class Iterator
    {
    public:
        Iterator(T*);
        Iterator(const Iterator&) = default;

        Iterator operator++(int) const;
        Iterator & operator++();
        Iterator const& operator++() const;
        
        T& operator*();
        T operator*() const;
        T* operator->();
        T const* operator->() const;

        bool operator!=(Iterator const &it) const;
    private:
        mutable T *ptr;
    };

    using ConstIterator = const Iterator;

    MyVector();
    MyVector(const MyVector &);
    MyVector& operator=(const MyVector &);
    ~MyVector();

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

    void push_back(const T&);

    MyVector operator+(const MyVector &) const;
private:
    void swap(MyVector &other);

    T *arr;
    int sz;
    int cap;
};

////////////////////////////////////

template<class T>
MyVector<T>::MyVector() :
    arr(nullptr),
    sz(0),
    cap(0)
{
}

template<class T>
MyVector<T>::MyVector(const MyVector<T> &other) :
    arr(nullptr),
    sz(other.sz),
    cap(other.cap)
{
    arr = reinterpret_cast<T*> (new char[cap * sizeof(T)]);
    for (int i = 0; i < sz; i++) {
        new (&arr[i]) T(other.arr[i]);
    }
}

template<class T>
void MyVector<T>::swap(MyVector<T> &other) {
    T *temp = arr;
    int temp_sz = sz;
    int temp_cap = cap;

    arr = other.arr;
    sz = other.sz;
    cap = other.cap;

    other.arr = temp;
    other.sz = temp_sz;
    other.cap = temp_cap;
}


template<class T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T> &other) {
    if (this == &other) {
        return *this;
    }

    MyVector<T> temp(other);
    swap(temp);
    return *this;
}

template<class T>
MyVector<T>::~MyVector() {
    for (int i = 0; i < sz; i++) {
        arr[i].~T();
    }
    delete[] reinterpret_cast<char*>(arr);
}


template<class T>
void MyVector<T>::push_back(const T &elem) {
    if (sz == cap) {
        cap = (cap == 0) ? 3 : (cap * 2);
        ++cap;
        T *temp = reinterpret_cast<T*> (new char[cap * sizeof(T)]);

        for (int i = 0; i < sz; i++) {
            new (&temp[i]) T(arr[i]);
        }

        for (int i = 0; i < sz; i++) {
            arr[i].~T();
        }
        delete[] reinterpret_cast<char*>(arr);

        arr = temp;
    }
    new (&arr[sz++]) T(elem);
}

template<class T>
MyVector<T>::Iterator MyVector<T>::begin() { return arr; }

template<class T>
MyVector<T>::Iterator MyVector<T>::end() { return (arr + sz); }

template<class T>
MyVector<T>::ConstIterator MyVector<T>::begin() const { return arr; }

template<class T>
MyVector<T>::ConstIterator MyVector<T>::end() const { return (arr + sz); }

template<class T>
MyVector<T> MyVector<T>::operator+(const MyVector<T> &other) const {
    if (sz != other.sz) {
        throw std::exception();
    }

    MyVector<T> sum(*this);
    MyVector<T> op(other);
    
    MyVector<T>::Iterator other_iter = op.begin();
    
    for (MyVector<T>::Iterator it = sum.begin(); it != sum.end(); ++it) {
        *it = *it + *other_iter++;
    }

    return sum;
}

/////////////////////////////////////////////

template<class T>
MyVector<T>::Iterator::Iterator(T *_ptr) :
    ptr(_ptr)
{
}

template<class T>
MyVector<T>::Iterator MyVector<T>::Iterator::operator++(int) const {
    Iterator temp(*this);
    ptr++;
    return temp;
}

template<class T>
MyVector<T>::Iterator& MyVector<T>::Iterator::operator++() {
    ptr++;
    return *this;
}

template<class T>
MyVector<T>::Iterator const& MyVector<T>::Iterator::operator++() const {
    ptr++;
    return *this;
}

template<class T>
T& MyVector<T>::Iterator::operator*() {
    return *ptr;
}

template<class T>
T MyVector<T>::Iterator::operator*() const {
    return *ptr;
}

template<class T>
T* MyVector<T>::Iterator::operator->() {
    return ptr;
}

template<class T>
T const* MyVector<T>::Iterator::operator->() const {
    return ptr;
}

template<class T>
bool MyVector<T>::Iterator::operator!=(typename MyVector<T>::Iterator const &iter) const {
    return ptr != iter.operator->();
}

///////////////////////

template<class T>
void square(MyVector<T> &vec) {
    throw std::exception();
}
template<>
void square(MyVector<int> &vec) {
    for (MyVector<int>::Iterator it = vec.begin(); it != vec.end(); it++) {
        *it = (*it) * (*it);
    }
}


int main() 
{
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);
    vec.push_back(8);


    std::cout << std::endl;
    MyVector<MyVector<int>> vecvec;
    vecvec.push_back(vec);
    vecvec.push_back(vec);
    vecvec.push_back(vec);
    vecvec.push_back(vec);
    vecvec.push_back(vec);
    MyVector<MyVector<int>> vecvec2;
    vecvec2 = vecvec2 = vecvec2 = vecvec;

    std::for_each(vecvec2.begin(), vecvec2.end(), 
    [](auto elem)
    {
        std::for_each(elem.begin(), elem.end(), 
        [](auto item)
        {
            std::cout << item << ' ';
        });
        std::cout << std::endl;
    });
    vecvec2 = vecvec2 + vecvec2;

    std::for_each(vecvec2.begin(), vecvec2.end(), 
    [](auto elem)
    {
        std::for_each(elem.begin(), elem.end(), 
        [](auto item)
        {
            std::cout << item << ' ';
        });
        std::cout << std::endl;
    });

    try
    {
        square(vec);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "=============" << std::endl;

    std::for_each(vec.begin(), vec.end(), 
    [](auto item)
    {
        std::cout << item << ' ';
    });
    std::cout << std::endl;

    try
    {
        square(vecvec2);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    for(MyVector<int>::ConstIterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    return 0;
}
