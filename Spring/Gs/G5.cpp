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
    virtual ~Number() = default;

    virtual int value() const;
    Data *clone() const;
private:
    int num;
};


class Text : public Data
{
public:
    Text(std::string);
    virtual ~Text() = default;

    virtual std::string value() const;
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
    void push_after_pop(Data*);
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
    sz(_sz),
    arr(nullptr)
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

void BoundedStack::push_after_pop(Data *data) {
    this->arr[pos++] = data;
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

class StackInterpreter
{
public:
    StackInterpreter(size_t);
    StackInterpreter(const StackInterpreter &) = delete;
    StackInterpreter& operator=(const StackInterpreter &) = delete;
    virtual ~StackInterpreter() = default;

    virtual void push(const Data &data);
    virtual Data *pop();
    void push_after_pop(Data *data);
private:
    BoundedStack stack;
};

class Instruction 
{
public:
    virtual void run (StackInterpreter &) = 0;
    virtual ~Instruction() = default;
private:
};

Number *pop_and_get_number(StackInterpreter &stack) {
    Data* pop_data = stack.pop();
    Number* num = dynamic_cast<Number*>(pop_data);
    if (num == nullptr) {
        stack.push_after_pop(pop_data);
        throw std::invalid_argument("On the stack NaN!");
    }
    return num;
}

class AddInstruction : public Instruction
{
public:
    void run (StackInterpreter &);
};



StackInterpreter::StackInterpreter(size_t _sz) : stack(_sz) {}
void StackInterpreter::push(const Data &data) {stack.push(data);}
void StackInterpreter::push_after_pop(Data *data) {stack.push_after_pop(data);}

Data *StackInterpreter::pop() {return stack.pop();}


void AddInstruction::run(StackInterpreter &stack) {
    Number *num1 = nullptr;
    Number *num2 = nullptr;
    try
    {
        num1 = pop_and_get_number(stack);
        num2 = pop_and_get_number(stack);
        stack.push(Number(num1->value() + num2->value()));
    }
    catch(const std::exception& e)
    {
        if (num1 != nullptr) {
            stack.push_after_pop(num1);
        }
        if (num2 != nullptr) {
            stack.push_after_pop(num2);
        }
        throw;
    }
    delete num1;
    delete num2;
}


#ifdef _main

int
main()
{
    try {
        StackInterpreter STACK(5);
        Number a(3);
        Number b(2);

        STACK.push(Text("text"));
        STACK.push(b);
        STACK.push(b);
        STACK.push(b);
        STACK.push(a);

        AddInstruction instr;
        instr.run(STACK);
        Data * elem = STACK.pop();
        std::cout << dynamic_cast<Number *>(elem)->value() << std::endl;
        delete elem;
        
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}
#endif