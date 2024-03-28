#include <iostream>
#include <cstring>
//Created (27.03)
//22:10 - 23:40 (27.03) #Removed all const :) First CE
//13:00 - 14:00 (28.03) #PS
//Some time in lesson :) #OK
//Summary 3:00

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
class Visitor;
class VirtualExpression 
{
public:
    virtual ~VirtualExpression() = default;
    virtual int get_priority() const = 0;
    virtual void get_infix(Visitor &visitor) const = 0;
    virtual VirtualExpression* clone() const = 0; 
private:
};

class VariableExpression: public VirtualExpression 
{
public:
    VariableExpression(IntVariable &);
    void get_infix(Visitor &Visitor) const;
    int get_priority() const;
    VirtualExpression *clone() const;
    const IntVariable & get_var() const {return var;}
private:
    IntVariable &var;
};

class LiteralExpression: public VirtualExpression 
{
public:
    LiteralExpression(int);
    void get_infix(Visitor &Visitor) const;
    int get_priority() const;
    VirtualExpression *clone() const;
    int get_num() const {return num;}
private:
    int num;
};

class BinaryExpression: public VirtualExpression
{
public:
    BinaryExpression(VirtualExpression *, VirtualExpression *);
    ~BinaryExpression();
    void get_infix(Visitor &Visitor) const;
    virtual char get_operand() const = 0;

    const VirtualExpression * get_left() const {return left;}
    const VirtualExpression * get_right() const {return right;}
protected:
    VirtualExpression *left;
    VirtualExpression *right;
};

class SumExpression: public BinaryExpression 
{
public:
    SumExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;
    
    VirtualExpression *clone() const;
private:
};

class SubExpression: public BinaryExpression 
{
public:
    SubExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;
    
    VirtualExpression *clone() const;
private:
};

class MultiplyExpression: public BinaryExpression 
{
public:
    MultiplyExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;

    VirtualExpression *clone() const;
private:
};

class DivisionExpression: public BinaryExpression 
{
public:
    DivisionExpression(VirtualExpression *_left, VirtualExpression *_right);
    int get_priority() const;
    char get_operand() const;

    VirtualExpression *clone() const;
private:
};

class NegativeExpression: public VirtualExpression 
{
public:
    NegativeExpression(VirtualExpression *);
    ~NegativeExpression();
    int get_priority() const;
    void get_infix(Visitor &Visitor) const;

    VirtualExpression *clone() const;
    const VirtualExpression *get_exp() const {return exp;}
private:
    VirtualExpression *exp;
};

class Expression 
{
public:
    Expression(int);
    Expression(IntVariable&);
    Expression(const Expression&);
    Expression(VirtualExpression *exp);

    ~Expression();

    int get_priority() const;
    void get_infix(Visitor &Visitor) const;
    Expression *clone() const;
    VirtualExpression *root() const;
private:
    VirtualExpression *exp_root;
};

class Visitor {
public:
    virtual void visit(const Expression&) = 0;
    virtual void visit(const VariableExpression&) = 0;
    virtual void visit(const LiteralExpression&) = 0;
    virtual void visit(const NegativeExpression&) = 0;
    virtual void visit(const BinaryExpression&) = 0;
    virtual ~Visitor() = default;
};
/*==========================================*/
VariableExpression::VariableExpression(IntVariable &_var):
    var(_var)
{
}

int VariableExpression::get_priority() const {return VAR_PR;}

void VariableExpression::get_infix(Visitor &visitor) const {
    visitor.visit(*this);
}

VirtualExpression *VariableExpression::clone() const {
    return new VariableExpression(var);
}
/*==========================================*/
LiteralExpression::LiteralExpression(int _num):
    num(_num)
{
}

int LiteralExpression::get_priority() const {return CONST_PR;}

void LiteralExpression::get_infix(Visitor &visitor) const {
    visitor.visit(*this);
}

VirtualExpression *LiteralExpression::clone() const {
    return new LiteralExpression(num);
}
/*==========================================*/
BinaryExpression::BinaryExpression(VirtualExpression *_left, VirtualExpression *_right):
    left(_left),
    right(_right)
{
}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

void BinaryExpression::get_infix(Visitor &visitor) const {
    visitor.visit(*this);
}
/*==========================================*/
SumExpression::SumExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int SumExpression::get_priority() const {return SUM_PR;}
char SumExpression::get_operand() const {return '+';}
VirtualExpression *SumExpression::clone() const {return new SumExpression(left->clone(), right->clone());}
/*==========================================*/
SubExpression::SubExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int SubExpression::get_priority() const {return SUB_PR;}
char SubExpression::get_operand() const {return '-';}

VirtualExpression *SubExpression::clone() const {return new SubExpression(left->clone(), right->clone());}
/*==========================================*/
MultiplyExpression::MultiplyExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int MultiplyExpression::get_priority() const {return MUL_PR;}
char MultiplyExpression::get_operand() const {return '*';}

VirtualExpression *MultiplyExpression::clone() const {return new MultiplyExpression(left->clone(), right->clone());}
/*==========================================*/
DivisionExpression::DivisionExpression(VirtualExpression *_left, VirtualExpression *_right):
    BinaryExpression(_left, _right)
{
}
int DivisionExpression::get_priority() const {return DIV_PR;}
char DivisionExpression::get_operand() const {return '/';}

VirtualExpression *DivisionExpression::clone() const {return new DivisionExpression(left->clone(), right->clone());}
/*==========================================*/
NegativeExpression::NegativeExpression(VirtualExpression *_exp):
    exp(_exp)
{
}

NegativeExpression::~NegativeExpression() {
    delete exp;
}

int NegativeExpression::get_priority() const {return NEG_PR;}

void NegativeExpression::get_infix(Visitor &visitor) const {
    visitor.visit(*this);
}

VirtualExpression *NegativeExpression::clone() const {return new NegativeExpression(exp->clone());}
/*==========================================*/
Expression::Expression(int var) : exp_root(new LiteralExpression(var)) {}
Expression::Expression(IntVariable &var) : exp_root(new VariableExpression(var)) {}
Expression::Expression(const Expression &other) : exp_root(other.exp_root->clone()) {}
Expression::Expression(VirtualExpression *exp) : exp_root(exp) {}

Expression::~Expression() {
    delete exp_root;
}

int Expression::get_priority() const {return exp_root->get_priority();}
void Expression::get_infix(Visitor &visitor) const {
    visitor.visit(*this);
}

Expression *Expression::clone() const {
    return new Expression(exp_root->clone());
}

VirtualExpression *Expression::root() const {return exp_root;}
/*==========================================*/
/*==========================================*/
/*==========================================*/
class PrettyPrinter : public Visitor
{
public:
    void visit(const Expression&);
    void visit(const VariableExpression&);
    void visit(const LiteralExpression&);
    void visit(const NegativeExpression&);
    void visit(const BinaryExpression&);
    PrettyPrinter& get_infix(const Expression &_exp);
    const Expression *get_exp() const {return exp;}
    
    int priority = DEFAULT_PR;
    std::ostream *out = nullptr;
    static const Expression *exp;
private:
};
const Expression *PrettyPrinter::exp = nullptr;

void PrettyPrinter::visit(const Expression &expression) {
    expression.root()->get_infix(*this);
}
void PrettyPrinter::visit(const VariableExpression &expression) {
    (*out) << expression.get_var().get_name();
}
void PrettyPrinter::visit(const LiteralExpression &expression) {
    (*out) << expression.get_num();
}
void PrettyPrinter::visit(const NegativeExpression &expression) {
    (*out) << "-";
    this->priority = NEG_PR;
    expression.get_exp()->get_infix(*this);
}
void PrettyPrinter::visit(const BinaryExpression &expression) {
    // std::cout << this->priority << ' ' << expression.get_priority() << std::endl;
    int exp_priority = expression.get_priority();
    bool printed = false;
    if (this->priority < exp_priority) {
        (*out) << '(';
        printed = true;
    }

    this->priority = exp_priority;
    expression.get_left()->get_infix(*this);
    
    (*out) << ' ' << expression.get_operand() << ' ';

    this->priority = exp_priority - 1;
    expression.get_right()->get_infix(*this);
    
    this->priority = exp_priority;

    if (printed) {
        (*out) << ')';
    }
}

PrettyPrinter& PrettyPrinter::get_infix(const Expression &_exp) {
    exp = &_exp;
    return *this;
}

std::ostream& operator<< (std::ostream &out, PrettyPrinter &printer) {
    printer.out = &out;
    printer.priority = DEFAULT_PR;
    printer.get_exp()->get_infix(printer);
    return out;
}
/*==========================================*/
Expression operator+ (Expression const& left, Expression const& right) { 
    
    return new SumExpression(left.root()->clone(), right.root()->clone());
}

Expression operator- (Expression const& left, Expression const& right) { 
    return new SubExpression(left.root()->clone(), right.root()->clone());
}

Expression operator* (Expression const& left, Expression const& right) { 
    return new MultiplyExpression(left.root()->clone(), right.root()->clone());
}

Expression operator/ (Expression const& left, Expression const& right) { 
    return new DivisionExpression(left.root()->clone(), right.root()->clone());
}

Expression operator- (Expression const& exp) { 
    return new NegativeExpression(exp.root()->clone());
}
} 


#ifdef _main
int main()
{
    using namespace equations;
    IntVariable x("x");
    IntVariable y("y");
    Expression expression = ((x + y) + x) * x - (7 - x) + -x;
    // Expression expression = (x + y) + x;


    PrettyPrinter printer;
    // Expression expression2 = expression - expression;
    std::cout << printer.get_infix(expression) << std::endl;
    // std::cout << printer.get_infix(expression2) << std::endl;
}
#endif