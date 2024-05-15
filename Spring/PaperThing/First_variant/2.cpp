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

        friend bool operator!=(Iterator const &it1, Iterator const &it2) {
            return it1.operator->() != it2.operator->();
        }


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

        std::cout << "wtf\n";
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
    vecvec2.push_back(MyVector<int>());
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


    return 0;
}
