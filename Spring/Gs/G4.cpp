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


class BoundedStack
{
public:
    explicit BoundedStack(size_t);
    BoundedStack(const BoundedStack &);
    BoundedStack& operator=(const BoundedStack&);
    ~BoundedStack();


    size_t size() const;
    void push(const Data &data);
    Data *pop();
private:
    size_t sz;
    size_t pos;
    Data **arr;
};


Number::Number(int _num) : num(_num) {}
int Number::value() const {return num;}
Data *Number::clone() const {return new Number(this->num);}


Text::Text(std::string _str) : str(_str) {}
std::string Text::value() const {return str;}
Data *Text::clone() const {return new Text(this->str);}


BoundedStack::BoundedStack(size_t _sz) :
    sz(_sz),
    pos(0)
{
    arr = new Data*[sz];
}
BoundedStack::BoundedStack(const BoundedStack &other) :
    sz(other.sz),
    pos(other.pos)
{
    arr = new Data*[sz];
    for (size_t i = 0; i < pos; i++) {
        arr[i] = other.arr[i]->clone();
    }
}

BoundedStack::~BoundedStack() {
    for (size_t i = 0; i < pos; i++) {
        delete arr[i];
    }
    delete[] arr;
}


BoundedStack& BoundedStack::operator=(const BoundedStack &other) {
    if (this == &other) {
        return *this;
    }

    for (size_t i = 0; i < pos; i++) {
        delete arr[i];
    }
    delete[] arr;
    sz = other.sz;
    pos = other.pos;

    arr = new Data*[sz];
    for (size_t i = 0; i < sz; i++) {
        arr[i] = other.arr[i]->clone();
    }
    return *this;
}

size_t BoundedStack::size() const {return sz;}

void BoundedStack::push(const Data &data) {
    if (pos == sz) {
        throw std::overflow_error("Stack overflow");
    }
    arr[pos++] = data.clone();
}

Data *BoundedStack::pop() {
    if (pos == 0) {
        throw std::range_error("Stack is empty");
    }
    return arr[--pos];
}


#ifdef _main
int main() {
    BoundedStack stack_f(2);

    stack_f.push(Number(1));
    stack_f.push(Text("second"));
    try
    {
        stack_f.push(Text("third"));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    BoundedStack stack(100);
    stack.push(Number(1));
    
    stack = stack_f = stack_f = stack_f = stack_f = stack_f = stack_f;
    
    std::cout << stack.size() << std::endl;

    Text *elem;
    Number *num;
    
    elem = static_cast<Text *>(stack.pop());
    std::cout << elem->value() << std::endl;
    delete elem;
    
    num = static_cast<Number *>(stack.pop());
    std::cout << num->value() << std::endl;
    delete num;

    try
    {
        num = static_cast<Number *>(stack.pop());
        std::cout << num->value() << std::endl;
        delete num;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;    
}
#endif