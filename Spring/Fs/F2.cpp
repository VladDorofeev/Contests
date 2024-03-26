#include <iostream>
#include <cstring>
//Created (25.03)
//20:00 - 21:30 (25.03) #First thinking, honestly 0:45 min
//10:30 - 11:30 (26.03) #Thinking about operators and structure
//22:20 - (26.03) #

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

class VirtualExpression 
{
public:
    virtual ~VirtualExpression() = default;
    virtual int get_priority() const = 0;
    virtual void get_infix(int priority, std::ostream &out) const = 0;
private:
};
/*==========================================*/
class VariableExpression: public VirtualExpression 
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
class LiteralExpression: public VirtualExpression 
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
class BinaryExpression: public VirtualExpression
{
public:
    BinaryExpression(VirtualExpression *, VirtualExpression *);
    ~BinaryExpression();
    void get_infix(int priority, std::ostream &out) const;
    virtual char get_operand() const = 0;
private:
    VirtualExpression *left;
    VirtualExpression *right;
};

BinaryExpression::BinaryExpression(VirtualExpression *_left, VirtualExpression *_right):
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
    SumExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

SumExpression::SumExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int SumExpression::get_priority() const {return SUM_PR;}
char SumExpression::get_operand() const {return '+';}
/*==========================================*/
class SubExpression: public BinaryExpression 
{
public:
    SubExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

SubExpression::SubExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int SubExpression::get_priority() const {return SUB_PR;}
char SubExpression::get_operand() const {return '-';}
/*==========================================*/
class MultiplyExpression: public BinaryExpression 
{
public:
    MultiplyExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

MultiplyExpression::MultiplyExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int MultiplyExpression::get_priority() const {return MUL_PR;}
char MultiplyExpression::get_operand() const {return '*';}
/*==========================================*/
class DivisionExpression: public BinaryExpression 
{
public:
    DivisionExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;
private:
};

DivisionExpression::DivisionExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int DivisionExpression::get_priority() const {return DIV_PR;}
char DivisionExpression::get_operand() const {return '/';}
/*==========================================*/
class NegativeExpression: public VirtualExpression 
{
public:
    NegativeExpression(VirtualExpression *);
    ~NegativeExpression();
    int get_priority() const;
    void get_infix(int priority, std::ostream &out) const;
private:
    VirtualExpression *exp;
};

NegativeExpression::NegativeExpression(VirtualExpression *_exp):
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
    const VirtualExpression & get_infix(const VirtualExpression  &exp) const;
private:
};

const VirtualExpression & PrettyPrinter::get_infix(const VirtualExpression &exp) const {
    return exp;
}

std::ostream& operator<< (std::ostream &out, const VirtualExpression &exp) {
    exp.get_infix(DEFAULT_PR, out);
    return out;
}
/*==========================================*/
/*==========================================*/
/*==========================================*/
class Expression : VirtualExpression
{
public:
    Expression(int);
    Expression(IntVariable&);
    Expression(const VirtualExpression&);
    int get_priority() const;
    void get_infix(int priority, std::ostream &out) const;
private:
    const VirtualExpression &exp;
};
Expression::Expression(int var) : exp(LiteralExpression(var)) {}
Expression::Expression(IntVariable &var) : exp(VariableExpression(var)) {}
Expression::Expression(const VirtualExpression &_exp) : exp(_exp) {}
int Expression::get_priority() const {return exp.get_priority();}
void Expression::get_infix(int priority, std::ostream &out) const {exp.get_infix(priority, out);}
/*==========================================*/
Expression operator+ (const Expression& left, const Expression & right) {
    return Expression(SumExpression(left, right));
}
} 

#ifdef _main
int main()
{
    using namespace equations;
    IntVariable x("x");
    // Expression expression = ((x + x) + x) * x - (7 - x);
    Expression expression = x + x;
    // Expression expression2 = expression - expression;
    PrettyPrinter printer;
    std::cout << printer.get_infix(expression) << std::endl;
    // std::cout << printer.get_infix(expression2) << std::endl;
}
#endif