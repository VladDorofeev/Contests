#include <iostream>
#include <cstring>


enum {SimplePriority, NegPriority, MulDivPriority, SumSubPriority};


namespace equations
{
    class IntVariable {
    public:
        IntVariable(char const*);
        IntVariable(IntVariable const&);
        ~IntVariable();
        
        IntVariable& operator=(IntVariable const&);

        char const* show_name_var() const;
    private:
        void swap(IntVariable);
        char* name_var;
    };


    class Expression {
    public:
        virtual void get_info(std::ostream&) const = 0;
        virtual int priority() const;
        virtual ~Expression();
    };


    class VariableExpression : public Expression {
    public:
        VariableExpression(IntVariable&);

        void get_info(std::ostream&) const override;
    private:
        IntVariable& _var;
    };


    class LiteralExpression : public Expression {
    public:
        LiteralExpression(int);

        void get_info(std::ostream&) const override;
    private:
        int _num;
    };


    class NegativeExpression : public Expression {
    public:
        NegativeExpression(Expression*);
        ~NegativeExpression();

        void get_info(std::ostream&) const override;
        int priority() const override;
    private:
        Expression* _exp_pt;
    };

    
    class DoubleVarExpression : public Expression {
    public:
        DoubleVarExpression(Expression*, Expression*, char);
        ~DoubleVarExpression();

        void get_info(std::ostream&) const final;
        int priority() const final;
    private:
        Expression* _left_exp;
        Expression* _right_exp;
        char _oper;
    };


    class SumExpression : public DoubleVarExpression {
    public:
        SumExpression(Expression*, Expression*);
    };


    class SubExpression : public DoubleVarExpression {
    public:
        SubExpression(Expression*, Expression*);
    };


    class MultiplyExpression : public DoubleVarExpression {
    public:
        MultiplyExpression(Expression*, Expression*);
    };


    class DivisionExpression : public DoubleVarExpression {
    public:
        DivisionExpression(Expression*, Expression*);
    };


    class ExpOutPut {
    public:
        ExpOutPut(Expression const&);
        ExpOutPut(ExpOutPut const&);
        Expression const* get_exp_pt() const;
    private:
        Expression const* _exp_pt;
    };


    class PrettyPrinter {
    public:
       ExpOutPut get_infix(Expression const&) const;
    };


    Expression *expr(IntVariable &v)
    {
        return new VariableExpression(v);
    }

    Expression *expr(int v)
    {
        return new LiteralExpression(v);
    }

    Expression *plus(Expression *left, Expression *right)
    {
        return new SumExpression(left, right);
    }

    Expression *minus(Expression *left, Expression *right)
    {
        return new SubExpression(left, right);
    }

    Expression *mult(Expression *left, Expression *right)
    {
        return new MultiplyExpression(left, right);
    }

    Expression *div(Expression *left, Expression *right)
    {
        return new DivisionExpression(left, right);
    }

    Expression *neg(Expression *operand)
    {
        return new NegativeExpression(operand);
    }

}

//Expression impl

equations::Expression::~Expression() {
}

int equations::Expression::priority() const {
    return SimplePriority;
}


//IntVariable impl

equations::IntVariable::IntVariable(char const* str)
    : name_var(new char [strlen(str) + 1])  
{
    strcpy(name_var, str);
}


equations::IntVariable::IntVariable(equations::IntVariable const& other)
    : name_var(new char [strlen(other.name_var) + 1]) 
{
    strcpy(name_var, other.name_var);
}


equations::IntVariable::~IntVariable() {
    delete[] name_var;
}


equations::IntVariable& equations::IntVariable::operator=(equations::IntVariable const& other){
    swap(other);
    return *this;
}


char const* equations::IntVariable::show_name_var() const {
    return name_var;
}


void equations::IntVariable::swap(equations::IntVariable other) {
    std::swap<char*>(name_var, other.name_var);
}


//LiteralExpression impl

equations::LiteralExpression::LiteralExpression(int num)
    : _num(num)
{
}


void equations::LiteralExpression::get_info(std::ostream& out) const {
    out << _num;
}


//VariableExpression impl

equations::VariableExpression::VariableExpression(equations::IntVariable& var)
    : _var(var) 
{
}


void equations::VariableExpression::get_info(std::ostream& out) const {
    out << _var.show_name_var();
}


//NegativeExpression impl

equations::NegativeExpression::NegativeExpression(equations::Expression* exp_pt)
    : _exp_pt(exp_pt) 
{
}


equations::NegativeExpression::~NegativeExpression() {
    delete _exp_pt;
}


void equations::NegativeExpression::get_info(std::ostream& out) const {
    out << '-';
    if (_exp_pt->priority() > priority()) {
        out << '(';
    }
    _exp_pt->get_info(out);
    if (_exp_pt->priority() > priority()) {
        out << ')';
    }
}


int equations::NegativeExpression::priority() const {
    return NegPriority;
}

//DoubleVarExpression imp

equations::DoubleVarExpression::DoubleVarExpression(equations::Expression* left_exp
    , equations::Expression* right_exp, char oper)
    : _left_exp(left_exp)
    ,_right_exp(right_exp)
    , _oper(oper) 
{
}


equations::DoubleVarExpression::~DoubleVarExpression() {
    delete _left_exp;
    delete _right_exp;
}


void equations::DoubleVarExpression::get_info(std::ostream& out) const {
    if (_left_exp->priority() > priority()) {
        out << '(';
    }
    _left_exp->get_info(out);
    if (_left_exp->priority() > priority()) {
        out << ')';
    }
    out << ' ' <<  _oper << ' ';
    if (_right_exp->priority() >= priority()) {
        out << '(';
    }
    _right_exp->get_info(out);
    if (_right_exp->priority() >= priority()) {
        out << ')';
    }
}


int equations::DoubleVarExpression::priority() const {
    if (_oper == '-' || _oper == '+') {
        return SumSubPriority;
    }
    return MulDivPriority;
}


//SumExpression impl 

equations::SumExpression::SumExpression(equations::Expression* left_exp
    , equations::Expression* right_exp) 
    : equations::DoubleVarExpression(left_exp, right_exp, '+')
{
}


//SubExpression impl 

equations::SubExpression::SubExpression(equations::Expression* left_exp
    , equations::Expression* right_exp) 
    : equations::DoubleVarExpression(left_exp, right_exp, '-')
{
}


//MultiplyExpression impl 

equations::MultiplyExpression::MultiplyExpression(equations::Expression* left_exp
    , equations::Expression* right_exp) 
    : equations::DoubleVarExpression(left_exp, right_exp, '*')
{
}


//DivisionExpression impl

equations::DivisionExpression::DivisionExpression(equations::Expression* left_exp
    , equations::Expression* right_exp) 
    : equations::DoubleVarExpression(left_exp, right_exp, '/')
{
}


//ExpOutPut impl

equations::ExpOutPut::ExpOutPut(Expression const& exp)
    : _exp_pt(&exp) 
{
}


equations::ExpOutPut::ExpOutPut(equations::ExpOutPut const& other)
    : _exp_pt(other._exp_pt) 
{
}


equations::Expression const* equations::ExpOutPut::get_exp_pt() const {
    return _exp_pt;
}


std::ostream& operator<<(std::ostream& out, equations::ExpOutPut const& exp_out) {
    exp_out.get_exp_pt()->get_info(out);
    return out;
}


//PrettyPrinter impl

equations::ExpOutPut equations::PrettyPrinter::get_infix(equations::Expression const& exp) const {
    return exp;
} 


int
main() {
    using namespace equations;
    IntVariable x("x"), y("y"), z("z");
    Expression const * expression =
        plus(
            minus(
                neg(expr(x)), 
                mult(
                    plus(
                        plus(expr(x), expr(x)),
                        plus(expr(-1), expr(y))),
                    expr(x))), 
             minus(expr(2), neg(expr(z))));
    PrettyPrinter printer;
    // -x - (x + x + (-1 + y)) * x + (2 - -z)
    std::cout << printer.get_infix(*expression) << std::endl;
    // using associativity:
    // ((x + y) + z) -> x + y + z
    // ((x + y) - z) -> x + y - z
    // (x + (y + z)) -> x + (y + z)
    // using precedence:
    // ((x * y) + z) -> x * y + z
    // (x * (y + z)) -> x * (y + z)
    delete expression;
}