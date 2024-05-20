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

    int capacity() const { return cap; }
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
    
    T *temp = new T[new_cap]; //can throw //gut
    
    for (int i = 0; i < other.sz; i++) {
        try
        {
            temp[i] = other.arr[i]; //can throw //gut
        }
        catch(...)
        {
            for (int j = 0; j < i; j++) {
                temp[j].~T();
            }
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
    for (int i = 0; i < sz; i++) {
        arr[i].~T();
    }

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
    catch(...)
    {
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

struct Type
{
public:
    Type() : item(0) {}
    Type(int num) : item(num) {}
    Type& operator= (const Type &other);
    ~Type() = default;

    int item;
    static int cnt;
};

int Type::cnt = 0;

Type& Type::operator= (const Type &other) {
    if (this  == &other) {
        return *this;
    }

    cnt++;
    if (cnt == 5) {
        throw std::logic_error("TYPE ENDED");
    }

    item = other.item;

    return *this;
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

    set = MultiSet<int> ();
    print(set);

    MultiSet<int> set2(set1, 123);
    print(set2);

    try
    {
        /* code */
        set2[1231] = 123;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    print(set2);


    std::cout << "---------------------" << std::endl;


    MultiSet<Type> typeset;
    try
    {
        typeset.add(Type(1));
        typeset.add(Type(2));
        typeset.add(Type(3));
        typeset.add(Type(4));
        typeset.add(Type(5));
        typeset.add(Type(6));
        typeset.add(Type(7));
        typeset.add(Type(8));
        typeset.add(Type(9));
        typeset.add(Type(10));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "==============================" << std::endl;
        if (typeset.size() == 0) {
            std::cout << "clear set " << std::endl;
            
        } else {
            std::cout << "sz = " << typeset.size() << " cap = " << typeset.capacity() << std::endl;
            for (int i = 0; i < typeset.size(); i++) {
                std::cout << typeset[i].item << ' ';
            }    
            std::cout << std::endl;
            std::cout << "==============================" << std::endl;
        }
    }
    
    

    return 0;
}