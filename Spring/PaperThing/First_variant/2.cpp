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

        Iterator operator++(int);
        Iterator& operator++();
        
        T& operator*();
        T operator*() const;
        T* operator->();
        T const* operator->() const;

    private:
        T *ptr;
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

private:
    void swap(MyVector &other);

    T *arr;
    int sz;
    int cap;
};



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
    arr = reinterpret_cast<T*> (new char[sz * sizeof(T)]);
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
    delete[] arr;
}


template<class T>
void MyVector<T>::push_back(const T &elem) {
    if (sz == cap) {
        cap = (cap==0) ? 100 : (cap * 2);

        T *temp = reinterpret_cast<T*> (new char[sz * sizeof(T)]);

        
        for (int i = 0; i < sz; i++) {
            new (&temp[i]) T(arr[i]);
        }

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
MyVector<T>::Iterator::Iterator(T *_ptr) :
    ptr(_ptr)
{
}

template<class T>
MyVector<T>::Iterator MyVector<T>::Iterator::operator++(int) {
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

template<typename T>
bool operator!=(typename MyVector<int>::Iterator &it1, typename MyVector<int>::Iterator &it2) {
    return it1.operator->() != it2.operator->();
}



int main() 
{
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);


    MyVector<int>::Iterator end = vec.end();
    for (MyVector<int>::Iterator iter = vec.begin(); iter != end; iter++) {
        std::cout << *iter << ' ';
    }
    std::cout << std::endl;

    // for (MyVector<int>::Iterator iter = vec.begin(); iter != vec.end(); ++iter) {
    //     std::cout << *iter << ' ';
    // }
    // std::cout << std::endl;


    // for (int elem : vec) {
    //     std::cout << elem << ' ';
        
    // }
    // std::cout << std::endl;

    // std::for_each(vec.begin(), vec.end(), 
    // [](auto elem)
    // {
    //     std::cout << elem << ' ';
    // });
    // std::cout << std::endl;


    return 0;
}
