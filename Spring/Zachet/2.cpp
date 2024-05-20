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


class Component
{
public:
    Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;

    int count() const;
    Component& subcomponent(int);
    const Component& subcomponent(int) const;
    void add(Component &);

    friend std::ostream& operator<<(std::ostream &, const Component &);
protected:
    MultiSet<Component *> set;
private:
    virtual const std::string& name() const = 0;
};

int Component::count() const { return set.size(); }

Component& Component::subcomponent(int index) {
    return *set[index];
}

const Component& Component::subcomponent(int index) const {
    return *set[index];
}

void Component::add(Component &elem) {
    set.add(&elem);
}

std::ostream& operator<<(std::ostream &in, const Component &component) {
    in << component.name();
    in << '(';
    for (int i = 0; i < component.count(); i++) {
        in << component.subcomponent(i);
        if (i != component.count() - 1) {
            in << ", ";
        }
    }
    in << ')';
    return in;
}

class Box : public Component
{
public:
    Box(const std::string &);
    Box(const Box &);
    Box& operator= (const Box &);
    ~Box() = default;
private:
    virtual const std::string& name() const;
    std::string s;
};

Box::Box(const std::string &_s) : 
    Component(),
    s(_s)
{
}

Box::Box(const Box &other) :
    Component(),
    s(other.s)
{
}

Box& Box::operator= (const Box &other) {
    if (this == &other) {
        return *this;
    }

    std::string temp(other.s);
    set.clear();
    s.swap(temp);

    return *this;
}

const std::string& Box::name() const {
    static std::string _name("qwerty");
    // return _name; 
    return s;
}


int main() {

    Box boxa("a");
    Box boxb("b");
    Box boxc("c");
    Box boxd("d");
    Box boxe("e");

    boxa.add(boxb);
    boxa.add(boxc);

    boxb.add(boxd);
    boxb.add(boxe);

    std::cout << boxa << std::endl;

    return 0;
}