#include <iostream>
#include <exception>

template <class T>
class MultiSet
{
public:
    MultiSet();
    MultiSet(const MultiSet&, int new_cap = -1);
    MultiSet& operator=(MultiSet);
    ~MultiSet();

    void clear();
    int size() const;

    T& operator[](int);
    const T& operator[](int) const;

    void add(const T&);
    void del(int, T &);

    int capacity() const { return cap; } // Not necessary
private:
    void swap(MultiSet&);

    T *arr;
    int sz;
    int cap;
};

template <class T>
MultiSet<T>::MultiSet() :
    arr(nullptr),
    sz(0),
    cap(0)
{
}

template <class T>
void MultiSet<T>::swap(MultiSet<T> &other) {
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


template <class T>
MultiSet<T>::MultiSet(const MultiSet<T>& other, int _new_cap) :
    arr(nullptr),
    sz(0),
    cap(0)
{ 
    int new_cap = ((_new_cap >= other.cap) && (_new_cap > 0)) ? _new_cap : other.cap;
    
    T *temp = new T[new_cap];
    
    for (int i = 0; i < other.sz; i++) {
        try
        {
            temp[i] = other.arr[i];
        }
        catch(const std::exception &err)
        {
            delete[] temp;
            throw;
        }
        
    }

    arr = temp;
    sz = other.sz;
    cap = new_cap;
}

template <class T>
MultiSet<T>& MultiSet<T>::operator=(MultiSet<T> other) {
    if (this == &other) {
        return *this;
    }
    swap(other);
    return *this;
}

template <class T>
MultiSet<T>::~MultiSet() {
    delete[] arr;
    sz = 0;
    cap = 0;
}

template <class T>
void MultiSet<T>::clear() { sz = 0; }

template <class T>
int MultiSet<T>::size() const { return sz; }

template <class T>
T& MultiSet<T>::operator[](int index){
    if (index >= sz) {
        throw std::out_of_range("Index >= sz");
    }
    return arr[index];
}

template <class T>
const T& MultiSet<T>::operator[](int index) const {
    if (index >= sz) {
        throw std::out_of_range("Index >= sz");
    }
    return arr[index];
}

template <class T>
void MultiSet<T>::add(const T &elem) {
    if (cap == sz) {
        int new_cap = (cap == 0) ? 2 : (cap * 2);

        MultiSet<T> temp(*this, new_cap);
        swap(temp);
    }

    try
    {
        arr[sz] = elem;
        sz++;
    }
    catch(const std::exception &err)
    {
        throw;
    }
}

template<class T>
void MultiSet<T>::del(int index, T &elem) {
    T saved_elem = (*this)[index];
    MultiSet<T> temp(*this);
    
    try
    {
        (*this)[index] = arr[sz-1];
        elem = saved_elem;
        sz--;
    }
    catch(const std::exception& e)
    {
        swap(temp);
        throw;
    }
    
}

template<class T>
void print(const MultiSet<T> &set) {
    std::cout << "==============================" << std::endl;
    if (set.size() == 0) {
        std::cout << "clear set " << std::endl;
        return;
    }

    std::cout << "sz = " << set.size() << " cap = " << set.capacity() << std::endl;
    for (int i = 0; i < set.size(); i++) {
        std::cout << set[i] << ' ';
    }    
    std::cout << std::endl;
    std::cout << "==============================" << std::endl;
}

int main() {
    MultiSet<int> set;
    set.add(1);    
    set.add(2);    
    set.add(3);    
    set.add(4);
    set.add(1);    
    set.add(2);    
    set.add(3);    
    set.add(4);
    set.add(1);    
    set.add(2);    
    set.add(3);    
    set.add(4);
    set.add(1);    
    set.add(2);    
    set.add(3);    
    set.add(4);

    print(set);

    MultiSet<int> set1(set);
    print(set1);

    set1.add(5);
    set = set = set = set = set1;
    print(set);

    MultiSet<int> set2(set1, 123);    

    try
    {
        set2[99084] = 123;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    print(set2);
    int a;
    set2.del(1, a);
    print(set2);
    std::cout << a << std::endl;

    set2.clear();
    set2 = set;
    print(set2);
    return 0;
}