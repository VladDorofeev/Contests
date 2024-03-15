#include <iostream>

namespace containers
{
class DynArray
{
public:
    DynArray();
    DynArray(const DynArray&);
    ~DynArray();
    
    DynArray& operator=(const DynArray &);

    int& operator[](int idx);
    int operator[](int idx) const;

    int size() const;

    //Make size bigger than before call method
    void expansion(int new_sz); 


    class Iterator
    {
    public:
        Iterator(int *);
        
        Iterator& operator++();
        int& operator*();
        const int& operator*() const;

        friend bool operator!= (const Iterator&, const Iterator&);
    private:
        int *p_arr;
    };

    Iterator begin_iterator();
    const Iterator begin_iterator() const;

    Iterator end_iterator();
    const Iterator end_iterator() const;
private:
    int *arr;
    int cap;
    int sz;
    enum { INIT_SZ = 100 }; 
};
DynArray::DynArray(): arr(nullptr), cap(0), sz(0) {}
DynArray::DynArray(const DynArray &d_arr):
    cap(d_arr.cap),
    sz(d_arr.sz) 
{        
    arr = new int[cap];
    for (int i = 0; i < sz; ++i) {
        arr[i] = d_arr[i];
    }
}
DynArray::~DynArray() {
    delete[] arr;
}

DynArray& 
DynArray::operator=(const DynArray &d_arr) {
    if (this == &d_arr) {
        return *this;
    }
    delete[] arr;
    cap = d_arr.cap;
    sz = d_arr.sz;
    arr = new int[cap];
    for (int i = 0; i < sz; ++i) {
        arr[i] = d_arr[i];
    }
    return *this;
}

int& 
DynArray::operator[](int idx) {
    return arr[idx];
}

int
DynArray::operator[](int idx) const {
    return arr[idx];
}

int 
DynArray::size() const {
    return sz;
}

void 
DynArray::expansion(int new_sz) {
    if (new_sz > cap) {
        if (cap == 0) {
            cap = INIT_SZ;
        }
        while (cap < 2 * new_sz) {
            cap *= 2;
        }

        int *temp = new int[cap];
        for (int i = 0; i < sz; ++i) {
            temp[i] = arr[i];
        }
        for (int i = sz; i < new_sz; ++i) {
            temp[i] = 0;
        }
        delete[] arr;
        arr = temp;
    } 
    sz = new_sz;   
}

DynArray::Iterator::Iterator(int *_p_arr):
    p_arr(_p_arr) 
{
}
        
DynArray::Iterator& DynArray::Iterator::operator++() {
    p_arr++;
    return *this;
}

int& 
DynArray::Iterator::operator*() {
    return *p_arr;
}

const int& 
DynArray::Iterator::operator*() const {
    return *p_arr;
}

bool 
operator!= (const DynArray::Iterator& l, const DynArray::Iterator& r) {
    return (l.p_arr != r.p_arr);
}

DynArray::Iterator 
DynArray::begin_iterator() {
    return DynArray::Iterator(arr);
}

const DynArray::Iterator
DynArray::begin_iterator() const {
    return DynArray::Iterator(arr);
}

DynArray::Iterator
DynArray::end_iterator() {
    return DynArray::Iterator(arr + sz);
}

const DynArray::Iterator 
DynArray::end_iterator() const {
    return DynArray::Iterator(arr + sz);
}

}

namespace iterators
{
    class InputIterator
    {
    public:
        InputIterator();
        InputIterator(std::istream &);

        const int& operator*() const;
        InputIterator operator++(int);
        std::istream *stream() const;
        bool fail() const;
    private:
        int num;
        std::istream *input;
        bool failed;
    };

    InputIterator::InputIterator(): 
        num(0), 
        input(nullptr), 
        failed(true) 
    {
    }

    InputIterator::InputIterator(std::istream &_input): 
        input(&_input) 
    {
        (*input) >> this->num;
        failed = input->fail();
    }

    const int& InputIterator::operator*() const {
        return num;
    }

    InputIterator 
    InputIterator::operator++(int) {
        InputIterator temp(*this);
        
        (*input) >> this->num;
        failed = input->fail();

        return temp;
    }
    std::istream *InputIterator::stream() const {
        return input;
    }
    bool InputIterator::fail() const {
        return failed;
    }


    bool operator!= (const InputIterator &left, const InputIterator &right) {
        if (left.fail() == right.fail()) {
            return false;
        }
        //need checking two streams
        return true;
    }

    class InserterIterator
    {
    public:
        InserterIterator(containers::DynArray&);
        InserterIterator(const InserterIterator&);

        InserterIterator& operator= (const InserterIterator &);

        int& operator* () const; 
        
        InserterIterator operator++(int);
    private:
        containers::DynArray &arr;
        int pos;
    };
    InserterIterator::InserterIterator(containers::DynArray &d_arr): 
        arr(d_arr), 
        pos(d_arr.size()) 
    {
    }

    InserterIterator::InserterIterator(const InserterIterator &iter): 
        arr(iter.arr), 
        pos(iter.pos) 
    {
    }

    InserterIterator& 
    InserterIterator::operator= (const InserterIterator &iter) {
        arr = iter.arr;
        pos = iter.pos;
        return *this;
    }

    int& 
    InserterIterator::operator* () const {
        if (pos >= arr.size()) {
            arr.expansion(pos + 1);
        }
        return this->arr[pos];
    } 

    InserterIterator 
    InserterIterator::operator++(int) {
        InserterIterator temp(*this);
        this->pos++;
        return temp;
    }
}


namespace algorithms
{
class IsEven
{
public:
    IsEven(bool cond);

    bool operator() (int) const;
private:
    bool condition;
};
IsEven::IsEven(bool cond) {
    condition = cond;
}

bool 
IsEven::operator() (int num) const {
    return (condition? (num % 2 == 0) : (num % 2 != 0));
}

class AddTo
{
public:
    AddTo(int &_summ);
    void operator() (int num) const;
private:
    int *summ;
};

AddTo::AddTo(int &_summ): summ(&_summ) {}
    
void 
AddTo::operator() (int num) const {
    *summ += num;
}
}

namespace algorithms
{
iterators::InserterIterator
copy(
    iterators::InputIterator iterator,
    iterators::InputIterator end,
    iterators::InserterIterator begin2)
{
    iterators::InserterIterator iterator2 = begin2;

    while (iterator != end) {
        *iterator2++ = *iterator++;
    }
    return iterator2;
}
}


#ifdef _main
namespace algorithms
{
void
foreach_if(
    containers::DynArray::Iterator iterator,
    containers::DynArray::Iterator end,
    algorithms::IsEven condition,
    algorithms::AddTo action)
{
    while (iterator != end) {
        if (condition(*iterator)) {
            action(*iterator);
        }
        ++iterator;
    }
}

}

int
main()
{
    containers::DynArray a;
    
    algorithms::copy(
        iterators::InputIterator(std::cin),
        iterators::InputIterator(),
        iterators::InserterIterator(a));
    
    int even_summ = 0, odd_summ = 0;

    algorithms::foreach_if(
        a.begin_iterator(),
        a.end_iterator(),
        algorithms::IsEven(true),
        algorithms::AddTo(even_summ));
    
    algorithms::foreach_if(
        a.begin_iterator(),
        a.end_iterator(),
        algorithms::IsEven(false),
        algorithms::AddTo(odd_summ));
    
    std::cout << even_summ << std::endl;
    std::cout << odd_summ << std::endl;
    return 0;
}
#endif