#include <iostream>
#include <string>

class Data
{
public:
    virtual ~Data() = default;
    virtual Data *clone() const = 0;
private:
};


class Number : public Data
{
public:
    Number(int);
    
    int value() const;
    Data *clone() const;
private:
    int num;
};


class Text : public Data
{
public:
    Text(std::string);

    std::string value() const;

    Data *clone() const;
private:
    std::string str;
};

class DataArr
{
public:
    explicit DataArr(size_t);
    DataArr(const DataArr &);
    DataArr& operator=(const DataArr &);
    virtual ~DataArr();

    void copy(const DataArr&);
    void swap(DataArr &);
    size_t arr_size() const;
protected:
    size_t sz;
    Data **arr;
};


class BoundedStack : private DataArr
{
public:
    explicit BoundedStack(size_t);
    BoundedStack(const BoundedStack &);
    BoundedStack& operator=(const BoundedStack&);
    ~BoundedStack() = default;
    

    size_t size() const;
    void push(const Data &data);
    Data *pop();
private:
    size_t pos;
};

Number::Number(int _num) : num(_num) {}
int Number::value() const {return num;}
Data *Number::clone() const {return new Number(this->num);}


Text::Text(std::string _str) : str(_str) {}
std::string Text::value() const {return str;}
Data *Text::clone() const {return new Text(this->str);}


DataArr::DataArr(size_t _sz) :
    sz(_sz)
{
    arr = new Data*[sz];
    for (size_t i = 0; i < sz; i++) {
        arr[i] = nullptr;
    }
}

DataArr::DataArr(const DataArr &other) :
    sz(0),
    arr(nullptr)
{
    DataArr temp(other.sz);
    temp.copy(other);
    this->swap(temp);
}

DataArr& DataArr::operator=(const DataArr &other) {
    if (this == &other) {
        return *this;
    }
    
    DataArr temp(other);
    this->swap(temp);

    return *this;
}

DataArr::~DataArr() {
    for (size_t i = 0; i < sz; i++) {
        if (arr[i] != nullptr) {
            delete arr[i];
        }
    }
    delete[] arr;
}

void DataArr::copy(const DataArr &other) {
    for (size_t i = 0; i < other.sz; i++) {
        if (other.arr[i] != nullptr) {
            arr[i] = other.arr[i]->clone();
        } else {
            arr[i] = nullptr;
        }
    }
}

void DataArr::swap(DataArr &other) {
    Data **temp_arr = other.arr;
    other.arr = arr;
    arr = temp_arr;

    size_t temp_sz = other.sz;
    other.sz = sz;
    sz = temp_sz;
}

size_t DataArr::arr_size() const {return sz;}


BoundedStack::BoundedStack(size_t _sz) :
    DataArr(_sz),
    pos(0)
{
}

BoundedStack::BoundedStack(const BoundedStack &other) :
    DataArr(other),
    pos(other.pos)
{
}

BoundedStack& BoundedStack::operator=(const BoundedStack &other) {
    if (this == &other) {
        return *this;
    }

    DataArr::operator=(other);

    pos = other.pos;

    return *this;
}

void BoundedStack::push(const Data &data) {
    if (pos >= this->arr_size()) {
        throw std::overflow_error("Stack overflow");
    }
    Data *temp = data.clone();
    this->arr[pos++] = temp;
}

Data *BoundedStack::pop() {
    if (pos == 0) {
        throw std::range_error("Stack is empty");
    }
    Data *temp = this->arr[--pos];
    this->arr[pos] = nullptr;
    return temp;
}

size_t BoundedStack::size() const {return pos;}


#ifdef _main
int main() {
    {
        BoundedStack stack(2);
        stack.push(Number(1));
        stack.push(Text("second"));
        try
        {
            stack.push(Text("third"));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

    }

    BoundedStack stack(100);
    stack.push(Number(1));
    
    stack = stack = stack = stack = stack = stack = stack;

    stack = BoundedStack(10);

    std::cout << stack.size() << std::endl;

    Text *elem;
    Number *num;
    stack.push(Number(1));
    stack.push(Text("2"));

    BoundedStack stack_cp(stack);

    elem = static_cast<Text*>(stack.pop());
    num = static_cast<Number*>(stack.pop());

    std::cout << elem->value() << std::endl;
    std::cout << num->value() << std::endl;

    delete elem;
    delete num;
    
    stack = stack_cp;

    elem = static_cast<Text*>(stack.pop());
    num = static_cast<Number*>(stack.pop());

    std::cout << elem->value() << std::endl;
    std::cout << num->value() << std::endl;

    delete elem;
    delete num;

    elem = static_cast<Text*>(stack_cp.pop());
    num = static_cast<Number*>(stack_cp.pop());

    std::cout << elem->value() << std::endl;
    std::cout << num->value() << std::endl;

    delete elem;
    delete num;

    return 0;    
}
#endif