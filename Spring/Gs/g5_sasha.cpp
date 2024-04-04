#include <iostream>
#include <exception>
#include <typeinfo>

class Data {
public:
    virtual ~Data() = default;
    virtual Data * clone() const = 0;
};

class Number: public Data {
public:
    Number(int);
    virtual ~Number() = default;
    virtual int value() const;
    Data * clone() const override;
private:
    int num;
};


class Text: public Data {
public:
    Text(std::string);
    virtual ~Text() = default;
    virtual std::string value () const;
    Data * clone() const override;
private:
    std::string str;
};


class StackInterpreter
{
public:
    StackInterpreter(size_t);
    StackInterpreter(StackInterpreter const&) = delete;
    StackInterpreter& operator=(StackInterpreter  const&) = delete;
    virtual ~StackInterpreter();

    virtual void push(Data const&);
    virtual Data * pop();
    void return_save(Data*);
private:
    Data ** data_arr;
    size_t size;
    size_t cur_pos;
};


class Instruction
{
public:
    virtual void run (StackInterpreter &) = 0;
    virtual ~Instruction() = default;
};




class AddInstruction: public Instruction
{
public:
    void run (StackInterpreter &);
};



Number::Number(int _num)
    : num(_num) 
{
}


int Number::value() const
{
    return num;
}


Data * Number::clone() const
{
    return new Number(num);
}


Text::Text(std::string _str)
    : str(_str)
{
}


std::string Text::value() const
{
    return str;
}


Data * Text::clone() const
{
    return new Text(str);
}


StackInterpreter::StackInterpreter(size_t _size)
    : data_arr(new Data*[_size])
    , size(_size)
    , cur_pos(0) 
{
    for(size_t i = 0; i < size; ++i) {
        data_arr[i] = nullptr;
    }
}


StackInterpreter::~StackInterpreter() {
    for (size_t i = 0; i < size; ++i) {
        delete data_arr[i];
    }
    delete [] data_arr;
}


void StackInterpreter::push(Data const& elem) {
    if (cur_pos >= size) {
        throw std::overflow_error("Overflow");
    }
    Data * tmp = elem.clone();
    data_arr[cur_pos++] = tmp;
}


Data * StackInterpreter::pop() {
    if (cur_pos == 0) {
        throw std::logic_error("Empty");
    }
    Data * tmp = data_arr[--cur_pos];
    data_arr[cur_pos] = nullptr;
    return tmp;
}


void StackInterpreter::return_save(Data* date) {
    data_arr[cur_pos++] = date;
}


Number* pop_and_get_number(StackInterpreter & stack)
{
    Data* data_pt = stack.pop();
    Number* number_pt = dynamic_cast<Number*>(data_pt);
    if (!number_pt) {
        stack.return_save(data_pt);
        throw std::exception();
    }
    return number_pt;
}


void AddInstruction::run(StackInterpreter & stack) {
    Number* num1 = nullptr;
    Number* num2 = nullptr;
    try {
        num1 = pop_and_get_number(stack);
        num2 = pop_and_get_number(stack);
        stack.push(Number(num1->value() + num2->value()));
    } catch (const std::exception &) {
        if (num2) {
            stack.return_save(num2);
        }
        if (num1) {
            stack.return_save(num1);
        }
        throw;
    }
    delete num1;
    delete num2;
}
