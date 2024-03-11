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
    const int& operator[](int idx) const;

    int size() const;

    //Make size bigger than before call method
    void expansion(int new_sz); 
private:
    int *arr;
    int cap;
    int sz;
    enum { INIT_SZ = 100 }; 
};
DynArray::DynArray(): arr(nullptr), cap(0), sz(0) {}
DynArray::DynArray(const DynArray &d_arr) {        
    cap = d_arr.cap;
    sz = d_arr.sz;
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
    if (idx >= sz) {
        throw "Out of bound";
    }
    return arr[idx];
}

const int& 
DynArray::operator[](int idx) const {
    if (idx >= sz) {
        throw "Out of bound";
    }
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
    private:
        int num;
        std::istream *input;
    };

    InputIterator::InputIterator(): num(0), input(nullptr) {};
    InputIterator::InputIterator(std::istream &_input): input(&_input) {
        if (!((*input) >> this->num)) {
            input = nullptr;
        }
    }

    const int& InputIterator::operator*() const {
        return num;
    }

    InputIterator 
    InputIterator::operator++(int) {
        InputIterator temp(*this);
        //if (input == nullptr) {
        //   throw "iterator`s stream closed";
        //}
        
        if (!((*input) >> num)) {
            input = nullptr;
        }

        return temp;
    }
    std::istream *InputIterator::stream() const {
        return input;
    }


    bool operator!= (const InputIterator &left, const InputIterator &right) {
        if ((left.stream() == nullptr) && (right.stream() == nullptr)) {
            return false;
        }
        if ((left.stream() == nullptr) || (right.stream() == nullptr)) {
            return true;
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
    InserterIterator::InserterIterator(containers::DynArray &d_arr): arr(d_arr), pos(0) {}

    InserterIterator::InserterIterator(const InserterIterator &iter): arr(iter.arr), pos(iter.pos) {}

    InserterIterator& 
    InserterIterator::operator= (const InserterIterator &iter) {
        arr = iter.arr;
        pos = iter.pos;
        return *this;
    }

    int& 
    InserterIterator::operator* () const {
        try
        {
            return this->arr[pos];
        }
        catch(const char *er)
        {
            arr.expansion(pos + 1);
            return this->arr[pos];
        }
    } 

    InserterIterator 
    InserterIterator::operator++(int) {
        InserterIterator temp(*this);
        this->pos++;
        return temp;
    }
} 



#ifdef _main
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

int
main()
{
    containers::DynArray a;
    algorithms::copy(
        iterators::InputIterator(std::cin),
        iterators::InputIterator(),
        iterators::InserterIterator(a));
    for (int i = 0; i < a.size(); ++i) {
        std::cout << a[i] << std::endl;
    }

}
#endif