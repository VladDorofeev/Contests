#include <iostream>
#include <cstring>
//Created (25.03)
//16:40 - 19:20 (25.03) #First OK 

namespace equations
{
    
enum 
{ 
    DEFAULT_PR = 100, 
    SUM_PR = 4, 
    SUB_PR = 4, 
    MUL_PR = 3, 
    DIV_PR = 3, 
    NEG_PR = 2, 
    VAR_PR = 1, 
    CONST_PR = 1, 
};

class IntVariable
{
public:
    IntVariable(const char *_name);
    IntVariable(const IntVariable &);
    IntVariable& operator= (const IntVariable &);
    ~IntVariable();
    
    const char *get_name() const;
private:
    char *name;
};
IntVariable::IntVariable(const char *_name) {
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
}
IntVariable::IntVariable(const IntVariable &other) {
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
}
IntVariable& IntVariable::operator= (const IntVariable &other) {
    if (this == &other) {
        return *this;
    }
    delete[] name;
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    return *this;
}
IntVariable::~IntVariable() {
    delete[] name;
}
const char *IntVariable::get_name() const {return name;}

/*==========================================*/
/*==========================================*/
/*==========================================*/

class Expression 
{
public:
    virtual ~Expression() = default;
    virtual int get_priority() const = 0;
    virtual void get_infix(int priority, std::ostream &out) const = 0;
private:
};
/*==========================================*/
class VariableExpression: public Expression 
{
public:
    VariableExpression(IntVariable &);
    void get_infix(int priority, std::ostream &out) const;
    int get_priority() const;
private:
    IntVariable &var;
};
VariableExpression::VariableExpression(IntVariable &_var):
    var(_var)
{
}

int VariableExpression::get_priority() const {return VAR_PR;}

void VariableExpression::get_infix(int priority, std::ostream &out) const {
    out << var.get_name();
}
/*==========================================*/
class LiteralExpression: public Expression 
{
public:
    LiteralExpression(int);
    void get_infix(int priority, std::ostream &out) const;
    int get_priority() const;
private:
    int num;
};
LiteralExpression::LiteralExpression(int _num):
    num(_num)
{
}

int LiteralExpression::get_priority() const {return CONST_PR;}


void LiteralExpression::get_infix(int priority, std::ostream &out) const {
    out << num;
}
/*==========================================*/
class BinaryExpression: public Expression
{
public:
    BinaryExpression(Expression *, Expression *);
    ~BinaryExpression();
    void get_infix(int priority, std::ostream &out) const;
    virtual char get_operand() const = 0;
private:
    Expression *left;
    Expression *right;
};

BinaryExpression::BinaryExpression(Expression *_left, Expression *_right):
    left(_left),
    right(_right)
{
}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

void BinaryExpression::get_infix(int priority, std::ostream &out) const {
    int this_priority = this->get_priority();
    if (priority < this_priority) {
        out << '(';
    }

    left->get_infix(this_priority, out);
    out << ' ' << this->get_operand() << ' ';
    right->get_infix(this_priority - 1, out);

    if (priority < this_priority) {
        out << ')';
    }
}
/*==========================================*/
class SumExpression: public BinaryExpression 
{
public:
    SumExpression(Expression *_left, Expression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

SumExpression::SumExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int SumExpression::get_priority() const {return SUM_PR;}
char SumExpression::get_operand() const {return '+';}
/*==========================================*/
class SubExpression: public BinaryExpression 
{
public:
    SubExpression(Expression *_left, Expression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

SubExpression::SubExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int SubExpression::get_priority() const {return SUB_PR;}
char SubExpression::get_operand() const {return '-';}
/*==========================================*/
class MultiplyExpression: public BinaryExpression 
{
public:
    MultiplyExpression(Expression *_left, Expression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

MultiplyExpression::MultiplyExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int MultiplyExpression::get_priority() const {return MUL_PR;}
char MultiplyExpression::get_operand() const {return '*';}
/*==========================================*/
class DivisionExpression: public BinaryExpression 
{
public:
    DivisionExpression(Expression *_left, Expression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

DivisionExpression::DivisionExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int DivisionExpression::get_priority() const {return DIV_PR;}
char DivisionExpression::get_operand() const {return '/';}
/*==========================================*/
class NegativeExpression: public Expression 
{
public:
    NegativeExpression(Expression *);
    ~NegativeExpression();
    int get_priority() const;
    void get_infix(int priority, std::ostream &out) const;
private:
    Expression *exp;
};

NegativeExpression::NegativeExpression(Expression *_exp):
    exp(_exp)
{
}

NegativeExpression::~NegativeExpression() {
    delete exp;
}

int NegativeExpression::get_priority() const {return NEG_PR;}

void NegativeExpression::get_infix(int priority, std::ostream &out) const {
    out << "-";
    exp->get_infix(NEG_PR, out);
}
/*==========================================*/
class PrettyPrinter
{
public:
    const Expression & get_infix(const Expression  &exp) const;
private:
};

const Expression & PrettyPrinter::get_infix(const Expression &exp) const {
    return exp;
}

std::ostream& operator<< (std::ostream &out, const Expression &exp) {
    exp.get_infix(DEFAULT_PR, out);
    return out;
}
/*==========================================*/
} 


#ifdef _main
namespace equations
{
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

int
main()
{
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
#endif