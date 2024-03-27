#include <iostream>
#include <cstring>
//Created (25.03)
//20:00 - 21:30 (25.03) #First thinking, honestly 0:45 min
//10:30 - 11:30 (26.03) #Thinking about operators and structure
//22:20 - 23:40 (26.03) #Try realiztion with VirtualExpression
//10:40 - 11:40 (27.03) #Some thinking about temporary obj
//21:10 - 22:10 (27.03) #First OK
//Summary 5:40

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
    Expression();
    Expression(int);
    Expression(IntVariable&);
    Expression(const Expression&);
    Expression(Expression *exp);

    virtual ~Expression();

    virtual int get_priority() const;
    virtual void get_infix(int priority, std::ostream &out) const;
    virtual Expression *clone() const;
    Expression *root() const;
private:
    Expression *exp_root;
};

/*==========================================*/
class VariableExpression: public Expression 
{
public:
    VariableExpression(IntVariable &);
    void get_infix(int priority, std::ostream &out) const;
    int get_priority() const;
    Expression *clone() const;
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

Expression *VariableExpression::clone() const {
    return new VariableExpression(var);
}

/*==========================================*/
class LiteralExpression: public Expression 
{
public:
    LiteralExpression(int);
    void get_infix(int priority, std::ostream &out) const;
    int get_priority() const;
    Expression *clone() const;
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

Expression *LiteralExpression::clone() const {
    return new LiteralExpression(num);
}

/*==========================================*/
class BinaryExpression: public Expression
{
public:
    BinaryExpression(Expression *, Expression *);
    ~BinaryExpression();
    void get_infix(int priority, std::ostream &out) const;
    virtual char get_operand() const = 0;

protected:
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
    
    Expression *clone() const;
private:
};

SumExpression::SumExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int SumExpression::get_priority() const {return SUM_PR;}
char SumExpression::get_operand() const {return '+';}
Expression *SumExpression::clone() const {return new SumExpression(left->clone(), right->clone());}

/*==========================================*/
class SubExpression: public BinaryExpression 
{
public:
    SubExpression(Expression *_left, Expression *_right);
    int get_priority() const;
    char get_operand() const;
    
    Expression *clone() const;
private:
};

SubExpression::SubExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int SubExpression::get_priority() const {return SUB_PR;}
char SubExpression::get_operand() const {return '-';}

Expression *SubExpression::clone() const {return new SubExpression(left->clone(), right->clone());}

/*==========================================*/
class MultiplyExpression: public BinaryExpression 
{
public:
    MultiplyExpression(Expression *_left, Expression *_right);
    int get_priority() const;
    char get_operand() const;

    Expression *clone() const;
private:
};

MultiplyExpression::MultiplyExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int MultiplyExpression::get_priority() const {return MUL_PR;}
char MultiplyExpression::get_operand() const {return '*';}

Expression *MultiplyExpression::clone() const {return new MultiplyExpression(left->clone(), right->clone());}
/*==========================================*/
class DivisionExpression: public BinaryExpression 
{
public:
    DivisionExpression(Expression *_left, Expression *_right);
    int get_priority() const;
    char get_operand() const;

    Expression *clone() const;
private:
};

DivisionExpression::DivisionExpression(Expression *_left, Expression *_right):
    BinaryExpression(_left, _right)
{
}
int DivisionExpression::get_priority() const {return DIV_PR;}
char DivisionExpression::get_operand() const {return '/';}

Expression *DivisionExpression::clone() const {return new DivisionExpression(left->clone(), right->clone());}

/*==========================================*/
class NegativeExpression: public Expression 
{
public:
    NegativeExpression(Expression *);
    ~NegativeExpression();
    int get_priority() const;
    void get_infix(int priority, std::ostream &out) const;

    Expression *clone() const;
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

Expression *NegativeExpression::clone() const {return new NegativeExpression(exp->clone());}

/*==========================================*/
class PrettyPrinter
{
public:
    const Expression & get_infix(const Expression  &exp) const;
};

const Expression & PrettyPrinter::get_infix(const Expression &exp) const {
    return exp;
}

std::ostream& operator<< (std::ostream &out, const Expression &exp) {
    exp.get_infix(DEFAULT_PR, out);
    return out;
}
/*==========================================*/
Expression::Expression() : exp_root(nullptr) {}
Expression::Expression(int var) : exp_root(new LiteralExpression(var)) {}
Expression::Expression(IntVariable &var) : exp_root(new VariableExpression(var)) {}
Expression::Expression(const Expression &other) : exp_root(other.exp_root->clone()) {}
Expression::Expression(Expression *exp) : exp_root(exp) {}

Expression::~Expression() {
    delete exp_root;
}

int Expression::get_priority() const {return exp_root->get_priority();}
void Expression::get_infix(int priority, std::ostream &out) const {
    exp_root->get_infix(priority, out);
}

Expression *Expression::clone() const {
    return new Expression(exp_root->clone());
}

Expression *Expression::root() const {return exp_root;}


/*==========================================*/
/*==========================================*/
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
    Expression expression = ((x + x) + x) * x - (7 - x) + -x;

    std::cout << "ended!\n";
    Expression expression2 = expression - expression;
    PrettyPrinter printer;
    std::cout << printer.get_infix(expression) << std::endl;
    std::cout << printer.get_infix(expression2) << std::endl;
}
#endif