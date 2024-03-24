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
        Expression();
        Expression(Expression*);
        Expression(Expression const&);
        Expression(IntVariable&);
        Expression(int);
        Expression* get_root_pt() const;
        virtual void get_info(std::ostream&) const;
        virtual int priority() const;
        virtual ~Expression();
        virtual Expression*  clone() const;
    private:
        Expression* _exp_root;
    };


    class VariableExpression : public Expression {
    public:
        VariableExpression(IntVariable&);

        void get_info(std::ostream&) const override;
        Expression* clone() const override ;
    private:
        IntVariable& _var;
    };


    class LiteralExpression : public Expression {
    public:
        LiteralExpression(int);

        void get_info(std::ostream&) const override;
        Expression* clone() const override;
    private:
        int _num;
    };


    class NegativeExpression : public Expression {
    public:
        NegativeExpression(Expression*);
        ~NegativeExpression();

        void get_info(std::ostream&) const override;
        int priority() const override;
        Expression* clone() const override;
    private:
        Expression* _exp_pt;
    };

    
    class DoubleVarExpression : public Expression {
    public:
        DoubleVarExpression(Expression*, Expression*, char);
        ~DoubleVarExpression();

        void get_info(std::ostream&) const final;
        int priority() const final;
        Expression* clone() const final;
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
}

//Expression impl

equations::Expression::Expression()
    :_exp_root(nullptr)
{
}

equations::Expression::Expression(equations::Expression* exp_pt)
    : _exp_root(exp_pt) 
{
} 

equations::Expression::Expression(equations::Expression const& other) 
    : _exp_root(other._exp_root->clone())
{  
}

equations::Expression::Expression(equations::IntVariable& int_var) 
    :_exp_root(new equations::VariableExpression(int_var))
{
}

equations::Expression::Expression(int num) 
    :_exp_root(new equations::LiteralExpression(num))
{
}

equations::Expression* equations::Expression::get_root_pt() const {
    return _exp_root;
}

void equations::Expression::get_info(std::ostream& out) const {
    _exp_root->get_info(out);
}

equations::Expression* equations::Expression::clone() const {
    return new Expression(_exp_root->clone());
}

equations::Expression::~Expression() {
    delete _exp_root;
}

int equations::Expression::priority() const {
    return SimplePriority;
}


equations::Expression operator+
    (equations::Expression const& exp1, equations::Expression const& exp2) { 
    return new equations::SumExpression(exp1.get_root_pt()->clone(), exp2.get_root_pt()->clone());
}

equations::Expression operator-
    (equations::Expression const& exp1, equations::Expression const& exp2) { 
    return new equations::SubExpression(exp1.get_root_pt()->clone(), exp2.get_root_pt()->clone());
}

equations::Expression operator*
    (equations::Expression const& exp1, equations::Expression const& exp2) { 
    return new equations::MultiplyExpression(exp1.get_root_pt()->clone(), exp2.get_root_pt()->clone());
}

equations::Expression operator/
    (equations::Expression const& exp1, equations::Expression const& exp2) { 
    return new equations::DivisionExpression(exp1.get_root_pt()->clone(), exp2.get_root_pt()->clone());
}

equations::Expression operator-
    (equations::Expression const& exp) { 
    return new equations::NegativeExpression(exp.get_root_pt()->clone());
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

equations::Expression* equations::LiteralExpression::clone() const {
    return new LiteralExpression(_num);
}

void equations::LiteralExpression::get_info(std::ostream& out) const {
    out << _num;
}


//VariableExpression impl

equations::VariableExpression::VariableExpression(equations::IntVariable& var)
    : _var(var) 
{
}


equations::Expression* equations::VariableExpression::clone() const {
    return new VariableExpression(_var);
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

equations::Expression* equations::NegativeExpression::clone() const {
    return new NegativeExpression(_exp_pt->clone());
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

equations::Expression* equations::DoubleVarExpression::clone() const {
    return new DoubleVarExpression(_left_exp->clone(), _right_exp->clone(), _oper);
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


/*int main()
{
    using namespace equations;
    IntVariable x("x");
    const Expression expression = ((-x + x) + x) / x - (7 - x);
    const Expression expression2 = expression - expression;
    PrettyPrinter printer;
    std::cout << printer.get_infix(expression) << std::endl;
    std::cout << printer.get_infix(expression2) << std::endl;
}*/

