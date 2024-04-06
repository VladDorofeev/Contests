#include <iostream>
#include <string>

enum {SimplePriority, NegPriority, MulDivPriority, SumSubPriority};

namespace equations
{
    class Variable {
    public:
        Variable(char const*);
        Variable(Variable const&) = default;
        Variable& operator=(Variable const&) = delete;

        void show_name(std::ostream&) const;
    private:
        char const* name_var;
    };

    template<typename T>
    class NegativeExpression {
    public:
        NegativeExpression(T const&);
        NegativeExpression(NegativeExpression<T> const&) = default;

        void get_info(std::ostream&) const;
        int priority() const;
    private:
        T exp;
    };


    template<typename T>
    class OneArgExpression {
    public:
        OneArgExpression(T const&);
        OneArgExpression(OneArgExpression<T> const&) = default;

        void get_info(std::ostream&) const;
        int priority() const;
    private:
        T liter;
    };


    template<typename T, typename U>
    class BinaryExpression {
    public:
        BinaryExpression(T const&, U const&, char);
        BinaryExpression(BinaryExpression<T, U> const&) = default;

        void get_info(std::ostream&) const;
        int priority() const;
    private:
        T left_exp;
        U right_exp;
        char oper;
    };

    template<typename T>
    OneArgExpression<T> expr(T const& elem) {
        return OneArgExpression(elem);
    }

    template<typename T, typename U>
    BinaryExpression<T, U> plus(T _left, U _right)
    {
        return BinaryExpression(_left, _right, '+');
    }

    template<typename T, typename U>
    BinaryExpression<T, U> minus(T _left, U _right)
    {
        return BinaryExpression(_left, _right, '-');
    }

    template<typename T, typename U>
    BinaryExpression<T, U> mult(T _left, U _right)
    {
        return BinaryExpression(_left, _right, '*');
    }

    template<typename T, typename U>
    BinaryExpression<T, U> div(T _left, U _right)
    {
        return BinaryExpression(_left, _right, '/');
    }

    template<typename T>
    NegativeExpression<T> neg(T ex)
    {
        return NegativeExpression(ex);
    }

    template<typename T>
    class ExpOutPut {
    public:
        ExpOutPut(T const&);
        ExpOutPut(ExpOutPut<T> const&) = default;
        void get_info(std::ostream&) const;
    private:
        T exp;
    };

    class PrettyPrinter {
    public:
        template<typename T>
        ExpOutPut<T> get_infix(T const&) const;
    };
    
}

//Variable impl
equations::Variable::Variable(char const* str)
    : name_var(str)
{
}


void equations::Variable::show_name(std::ostream& out) const {
    out << name_var;
}


std::ostream& operator<<(std::ostream& out, equations::Variable const& var) {
    var.show_name(out);
    return out;
}



//NegativeExpression impl
template<typename T>
equations::NegativeExpression<T>::NegativeExpression(T const& _exp)
    : exp(_exp) 
{
}


template<typename T>
int equations::NegativeExpression<T>::priority() const {
    return NegPriority;
}


template<typename T>
void equations::NegativeExpression<T>::get_info(std::ostream& out) const {
    out << '-';
    if (exp.priority() > priority()) {
        out << '(';
    }
    exp.get_info(out);
    if (exp.priority() > priority()) {
        out << ')';
    }
}


//OneArgExpression impl
template<typename T>
equations::OneArgExpression<T>::OneArgExpression(T const& _liter)
    : liter(_liter)
{
}


template<typename T>
int equations::OneArgExpression<T>::priority() const {
    return SimplePriority;
}


template<typename T>
void equations::OneArgExpression<T>::get_info(std::ostream& out) const {
    out << liter;
}


//BinaryExpression impl
template<typename T, typename U>
equations::BinaryExpression<T, U>::BinaryExpression(T const& _left, U const& _right, char _oper)
    : left_exp(_left)
    , right_exp(_right)
    , oper(_oper)
{
}

template<typename T, typename U>
void equations::BinaryExpression<T, U>::get_info(std::ostream& out) const {
    if (left_exp.priority() > priority()) {
        out << '(';
    }
    left_exp.get_info(out);
    if (left_exp.priority() > priority()) {
        out << ')';
    }
    out << ' ' <<  oper << ' ';
    if (right_exp.priority() >= priority()) {
        out << '(';
    }
    right_exp.get_info(out);
    if (right_exp.priority() >= priority()) {
        out << ')';
    }
}

template<typename T, typename U>
int equations::BinaryExpression<T, U>::priority() const {
    switch (oper)
    {
    case '+':
    case '-':
        return SumSubPriority;
    case '*':
    case '/':
        return MulDivPriority; 
    default:
        break;
    }
    return 0;
}

//ExpOutPut impl 
template<typename T>
equations::ExpOutPut<T>::ExpOutPut(T const& _exp)
    : exp(_exp)
{
}

template<typename T>
void equations::ExpOutPut<T>::get_info(std::ostream& out) const {
    exp.get_info(out);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, equations::ExpOutPut<T> const& output) {
    output.get_info(out);
    return out;
}

//PrettyPrinter impl
template<typename T>
equations::ExpOutPut<T> equations::PrettyPrinter::get_infix(T const& _exp) const {
    return ExpOutPut<T>(_exp);
}


int main()
{
    using namespace equations;
    using namespace std::string_literals;
    Variable x("x"), y("y"), z("z");
    PrettyPrinter printer;
    // -x - (x + x + (-1 + y)) * x + (rty - -z)
    std::cout << printer.get_infix(
        plus(
            minus(
                neg(expr(x)), 
                mult(
                    plus(
                        plus(expr(x), expr(x)),
                        plus(expr(-1), expr(y))),
                    expr(x))), 
             minus(expr("rty"s), neg(expr(z))))
        ) << std::endl;

}    