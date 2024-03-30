#include <iostream>
#include <exception>

class Data {
public:
    virtual ~Data() = default;

    virtual Data* clone() const = 0;
};


class Number : public Data {
public:
    Number(int);

    int value() const;
    Data* clone() const override;
private:
    int num;
};


class Text : public Data {
public:
    Text(std::string);

    std::string value() const;
    Data* clone() const final;
private:
    std::string text;
};


class BoundedStack {
public:
    BoundedStack(size_t);
    BoundedStack(BoundedStack const&);

    BoundedStack& operator=(BoundedStack const&);

    size_t size() const;
    void push(Data const&);
    Data* pop();
private:
    class DataArr {
    public:
        DataArr(size_t);
        ~DataArr();

        void copy(DataArr const&);
        Data*& operator[](size_t);
        Data const* const& operator[](size_t) const;
        void swap(DataArr&);
    private:
        Data** arr;
        size_t _size;
    };
    DataArr data_arr;
    size_t _size;
    size_t cur_pos;
};

//Number impl
Number::Number(int _num)
    : num(_num) 
{
}


int Number::value() const {
    return num;
}


Data* Number::clone() const {
    return new Number(num);
}


//Text impl
Text::Text(std::string _text)
    : text(_text) 
{
}


std::string Text::value() const {
    return text;
}


Data* Text::clone() const {
    return new Text(text);
}
//DataArr impl
BoundedStack::DataArr::DataArr(size_t size)
    : arr(new Data*[size])
    , _size(size)
{
    for(size_t i = 0; i < _size; ++i) {
        arr[i] = nullptr;
    }
}


BoundedStack::DataArr::~DataArr() {
    for(size_t i = 0; i < _size; ++i) {
        delete arr[i];
    }
    delete[] arr;
}


void BoundedStack::DataArr::copy(DataArr const& other) {
    for(size_t i = 0; i < _size; ++i) {
        if (other.arr[i]) {
            arr[i] = other.arr[i]->clone();
        }
    }
}


Data*& BoundedStack::DataArr::operator[](size_t pos) {
    return arr[pos];
}

Data const* const& BoundedStack::DataArr::operator[](size_t pos) const {
    return arr[pos];
}


void BoundedStack::DataArr::swap(DataArr& other) {
    Data** tmp = arr;
    arr = other.arr;
    other.arr = tmp;
    int tmp_n = _size;
    _size = other._size;
    other._size = tmp_n;
}


//BoundedStack impl
BoundedStack::BoundedStack(size_t size) try
    : data_arr(size)
    , _size(size)
    , cur_pos(0)
{
} catch(std::exception&) {
    throw;
}


BoundedStack::BoundedStack(BoundedStack const& other) try
    : data_arr(other._size)
    , _size(other._size)
    , cur_pos(other.cur_pos)
{
    data_arr.copy(other.data_arr);
} catch(std::exception&) {
    throw;
}


BoundedStack& BoundedStack::operator=(BoundedStack const& other) {
    DataArr tmp_arr(other._size);
    tmp_arr.copy(other.data_arr);
    data_arr.swap(tmp_arr);
    _size = other._size;
    cur_pos = other.cur_pos;
    return *this;
}


size_t BoundedStack::size() const {
    return cur_pos;
}


void BoundedStack::push(const Data & num_text) {
    if (cur_pos >= _size) {
        throw std::exception();
    }
    data_arr[cur_pos++] = num_text.clone();
}


Data * BoundedStack::pop() {
    if (cur_pos == 0) {
        throw std::exception();
    }
    Data * num_text = data_arr[--cur_pos];
    data_arr[cur_pos] = nullptr;
    return num_text;
}