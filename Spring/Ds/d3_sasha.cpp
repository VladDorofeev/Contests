#include <iostream>

namespace containers {
    class DynArray {
    public:
        DynArray();
        DynArray(const DynArray&);
        ~DynArray();
        DynArray& operator=(const DynArray& other);
        int& operator[](int);
        const int& operator[](int) const;
        void increase_size(int);
        int size() const;
        class Iterator {
        public:
            Iterator(int *);
            Iterator(const Iterator&);
            int* get_arr_pt() const;
            Iterator& operator++();
            int& operator*();
            const int& operator*() const;
        private:
            int* it_array;
        };
        Iterator begin_iterator();
        Iterator end_iterator();
    private:
        int* array;
        int capacity;
        int cnt;
    };
}
//containers def
//DynArray def
containers::DynArray::DynArray(): array(nullptr), capacity(0), cnt(0) {
}
containers::DynArray::DynArray(const containers::DynArray& other)
    : array(other.capacity > 0?new int[other.capacity]:nullptr), capacity(other.capacity), cnt(other.cnt) {
    for(int i = 0; i < cnt; ++i) {
        array[i] = other.array[i];
    }
}
containers::DynArray::~DynArray() {
    delete[] array;
}

containers::DynArray& containers::DynArray::operator=(const containers::DynArray& other) {
    if (array != other.array) {
        delete[] array;
        capacity = other.capacity;
        cnt = other.cnt;
        array = capacity>0?new int[capacity]:nullptr;
        for(int i = 0; i < cnt; ++i) {
            array[i] = other.array[i];
        }
    }
    return *this;
}
int& containers::DynArray::operator[](int index) {
    return array[index];
}
const int& containers::DynArray::operator[](int index) const {
    return array[index];
}
void containers::DynArray::increase_size(int pos) {
    if (capacity <= pos) {
        if (capacity == 0) {
            capacity = 1;
        }
        while(capacity <= pos) {
            capacity = 2 * capacity;
        }
        ++capacity;
        int* new_array = new int[capacity];
        for(int i = 0; i < capacity; ++i) {
            new_array[i] = 0;
        }
        for(int i = 0; i < cnt; ++i) {
            new_array[i] = array[i];
        }
        delete[] array;
        array = new_array;
    }
    if (cnt <= pos) {
        cnt = pos + 1;
    }
}

int containers::DynArray::size() const{
    return cnt;
}
containers::DynArray::Iterator containers::DynArray::begin_iterator() {
    return array;
}
containers::DynArray::Iterator containers::DynArray::end_iterator() {
    return array?array + cnt:nullptr;
}
//DynArray::Iterator def
containers::DynArray::Iterator::Iterator(int* arr): it_array(arr) {
}
containers::DynArray::Iterator::Iterator(const containers::DynArray::Iterator& other)
    : it_array(other.it_array) {
}

int* containers::DynArray::Iterator::get_arr_pt() const {
    return it_array;
}
bool operator!=
    (const containers::DynArray::Iterator& it1, const containers::DynArray::Iterator& it2) {
    return it1.get_arr_pt() != it2.get_arr_pt();
}
containers::DynArray::Iterator& containers::DynArray::Iterator::operator++() {
    ++it_array;
    return (*this);
}
int& containers::DynArray::Iterator::operator*() {
    return *it_array;
}
const int& containers::DynArray::Iterator::operator*() const {
    return *it_array;
}

namespace iterators{
    class InputIterator {
    public:
        InputIterator();
        InputIterator(std::istream&);
        InputIterator(const InputIterator&);
        std::istream* get_stream_pt() const;
        int& operator*();
        const int& operator*() const;
        InputIterator operator++(int);
    private:
        std::istream* stream_pt;
        int cur_num;
    };

    class InserterIterator {
    public:
        InserterIterator();
        InserterIterator(containers::DynArray&);
        InserterIterator(const InserterIterator&);
        int& operator*();
        const int& operator*() const;
        InserterIterator operator++(int);
    private:
        containers::DynArray* array;
        int cur_pos;
    };
}

//iterators def
//InputIterator def
iterators::InputIterator::InputIterator(): stream_pt(nullptr) {
}
iterators::InputIterator::InputIterator(std::istream& in): stream_pt(&in) {
    stream_pt = *stream_pt >> cur_num?&in:nullptr;
}
iterators::InputIterator::InputIterator(const iterators::InputIterator& other)
    : stream_pt(other.stream_pt), cur_num(other.cur_num) {
}

std::istream* iterators::InputIterator::get_stream_pt() const{
    return stream_pt;
}
bool operator!=(const iterators::InputIterator& it1, const iterators::InputIterator& it2) {
    return it1.get_stream_pt() != it2.get_stream_pt();
}

int& iterators::InputIterator::operator*() {
    return cur_num;
}
const int& iterators::InputIterator::operator*() const {
    return cur_num;
}
iterators::InputIterator iterators::InputIterator::operator++(int) {
    iterators::InputIterator prev(*this);
    if (stream_pt) {
        stream_pt = *stream_pt >> cur_num?stream_pt:nullptr;
    }
    return prev;
}
//InsertIterator def
iterators::InserterIterator::InserterIterator(): array(nullptr), cur_pos(0) {
}
iterators::InserterIterator::InserterIterator(containers::DynArray& arr)
    : array(&arr), cur_pos(0) {
}
iterators::InserterIterator::InserterIterator(const iterators::InserterIterator& other)
    : array(other.array), cur_pos(other.cur_pos) {
}

int& iterators::InserterIterator::operator*() {
    array->increase_size(cur_pos);
    return (*array)[cur_pos];
}
const int& iterators::InserterIterator::operator*() const {
    array->increase_size(cur_pos);
    return (*array)[cur_pos];
}
iterators::InserterIterator iterators::InserterIterator::operator++(int) {
    iterators::InserterIterator prev(*this);
    cur_pos++;
    return prev;
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
    class IsEven {
    public:
        IsEven(bool);
        IsEven(const IsEven&);
        bool operator()(int) const;
    private:
        bool is_even;
    };
    class AddTo {
    public:
        AddTo(int&);
        AddTo(const AddTo&);
        void operator()(int) const;
    private:
        int* add_to;
    };

}
//algorithms def
//IsEven def
algorithms::IsEven::IsEven(bool even): is_even(even) {
}
algorithms::IsEven::IsEven(const algorithms::IsEven& other): is_even(other.is_even) {
}

bool algorithms::IsEven::operator()(int num) const {
    return is_even?num % 2 == 0:num % 2 != 0;
}

//AddTo def
algorithms::AddTo::AddTo(int& var): add_to(&var) {
}
algorithms::AddTo::AddTo(const algorithms::AddTo& other): add_to(other.add_to) {
}

void algorithms::AddTo::operator()(int num) const {
    *add_to += num;
}
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
}