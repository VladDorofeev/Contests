#include <iostream>
#include <cstring>


enum {SimplePriority, NegPriority, MulDivPriority, SumSubPriority, TestMode, SolutionMode};


namespace equations
{
    class IntVariable {
    public:
        IntVariable(char const*, int, int);
        IntVariable(IntVariable const&);
        ~IntVariable();
        
        IntVariable& operator=(IntVariable const&);

        char const* show_name_var() const;
        int get_low_lim() const;
        int get_up_lim() const;
        void set_value(int);
        int get_value() const;
    private:
        void swap(IntVariable);
        char* name_var;
        int lower_limit;
        int upper_limit;
        int value;
    };

    class Visitor;

    class Expression {
    public:
        Expression();
        Expression(Expression*);
        Expression(Expression const&);
        Expression(IntVariable&);
        Expression(int);
        Expression* get_root_pt() const;
        virtual void get_info(Visitor&) const;
        virtual int priority() const;
        virtual ~Expression();
        virtual Expression*  clone() const;
    private:
        Expression* _exp_root;
    };


    class VariableExpression : public Expression {
    public:
        VariableExpression(IntVariable&);
        IntVariable* get_var() const; 
        void get_info(Visitor&) const override;
        Expression* clone() const override ;
    private:
        IntVariable* _var;
    };


    class LiteralExpression : public Expression {
    public:
        LiteralExpression(int);
        int get_num() const;
        void get_info(Visitor&) const override;
        Expression* clone() const override;
    private:
        int _num;
    };


    class NegativeExpression : public Expression {
    public:
        NegativeExpression(Expression*);
        ~NegativeExpression();
        Expression const* get_exp_pt() const;
        void get_info(Visitor&) const override;
        int priority() const override;
        Expression* clone() const override;
    private:
        Expression* _exp_pt;
    };

    
    class DoubleVarExpression : public Expression {
    public:
        DoubleVarExpression(Expression*, Expression*, char);
        ~DoubleVarExpression();
        Expression const* get_left_exp_pt() const;
        Expression const* get_right_exp_pt() const;
        char get_oper() const;
        void get_info(Visitor&) const final;
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

    class Equation {
    public:
        Equation(Expression const*, Expression const*);
        ~Equation();
        Expression const* get_left_exp() const;
        Expression const* get_right_exp() const;
    private:
        Expression const* _left_exp;
        Expression const* _right_exp;
    };

    int solve(Equation const&);


    class PrettyPrinter;

    class ExpOutPut {
    public:
        ExpOutPut(Expression const&, PrettyPrinter *);
        ExpOutPut(ExpOutPut const&);
        Expression const& get_exp_pt() const;
        PrettyPrinter * get_printer() const;
    private:
        Expression const& _exp;
        PrettyPrinter* _printer_pt;
    };

    class Visitor {
    public:
        virtual void visit(Expression const&) = 0;
        virtual void visit(VariableExpression const&) = 0;
        virtual void visit(LiteralExpression const&) = 0;
        virtual void visit(NegativeExpression const&) = 0;
        virtual void visit(DoubleVarExpression const&) = 0;
        virtual ~Visitor() = default;
    };

    class PrettyPrinter : public Visitor {
    public:
        PrettyPrinter();
        ExpOutPut get_infix(Expression const&);
        void set_out(std::ostream&);
        void visit(Expression const&) override;
        void visit(VariableExpression const&) override;
        void visit(LiteralExpression const&) override;
        void visit(NegativeExpression const&) override;
        void visit(DoubleVarExpression const&) override;
    private:
        std::ostream* _out;
    };

    class PrettySolver : public Visitor {
    public:
        PrettySolver(Equation const&);
        bool failed() const;
        void change_mode();
        IntVariable* get_var();
        void set_num(int);
        void clear_fail();
        void clear_sum();
        int get_sum() const;
        void visit(Expression const&) final;
        void visit(VariableExpression const&) final;
        void visit(LiteralExpression const&) final;
        void visit(NegativeExpression const&) final;
        void visit(DoubleVarExpression const&) final;
    private:
        Expression const* _left_exp;
        Expression const* _right_exp;
        IntVariable* _var_pt;
        int sum;
        int mode;
        bool fail;
        int num;
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


void equations::Expression::get_info
    (equations::Visitor& pr) const {
    pr.visit(*this);
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


equations::Equation operator==
    (equations::Expression const& exp1, equations::Expression const& exp2) {
    return equations::Equation(exp1.get_root_pt()->clone(), exp2.get_root_pt()->clone());
}


//IntVariable impl

equations::IntVariable::IntVariable(char const* str, int low_lim, int up_lim)
    : name_var(new char [strlen(str) + 1]) 
    , lower_limit(low_lim)
    , upper_limit(up_lim) 
{
    strcpy(name_var, str);
}


equations::IntVariable::IntVariable(equations::IntVariable const& other)
    : name_var(new char [strlen(other.name_var) + 1]) 
    , lower_limit(other.lower_limit)
    , upper_limit(other.upper_limit)
{
    strcpy(name_var, other.name_var);
}


equations::IntVariable::~IntVariable() {
    delete[] name_var;
}


equations::IntVariable& equations::IntVariable::operator=(equations::IntVariable const& other){
    if (this != &other) {
        swap(other);
    }
    return *this;
}


char const* equations::IntVariable::show_name_var() const {
    return name_var;
}


int equations::IntVariable::get_low_lim() const {
    return lower_limit;
}


int equations::IntVariable::get_up_lim() const {
    return upper_limit;
}


void equations::IntVariable::set_value(int num) {
    value = num;
}


void equations::IntVariable::swap(equations::IntVariable other) {
    std::swap<char*>(name_var, other.name_var);
    lower_limit = other.lower_limit;
    upper_limit = other.upper_limit;
}

int equations::IntVariable::get_value() const {
    return value;
}

std::ostream& operator<<(std::ostream& out, equations::IntVariable const& var) {
    return out << var.get_value();
}


//LiteralExpression impl

equations::LiteralExpression::LiteralExpression(int num)
    : _num(num)
{
}


int equations::LiteralExpression::get_num() const {
    return _num;
}


equations::Expression* equations::LiteralExpression::clone() const {
    return new LiteralExpression(_num);
}

void equations::LiteralExpression::get_info
    (equations::Visitor& pr) const {
    pr.visit(*this);
}




//VariableExpression impl

equations::VariableExpression::VariableExpression(equations::IntVariable& var)
    : _var(&var) 
{
}

equations::IntVariable* equations::VariableExpression::get_var() const {
    return _var;
}

equations::Expression* equations::VariableExpression::clone() const {
    return new VariableExpression(*_var);
}

void equations::VariableExpression::get_info
    (equations::Visitor& pr) const {
    pr.visit(*this);
}


//NegativeExpression impl

equations::NegativeExpression::NegativeExpression(equations::Expression* exp_pt)
    : _exp_pt(exp_pt) 
{
}


equations::NegativeExpression::~NegativeExpression() {
    delete _exp_pt;
}

equations::Expression const* equations::NegativeExpression::get_exp_pt() const {
    return _exp_pt;
}

equations::Expression* equations::NegativeExpression::clone() const {
    return new NegativeExpression(_exp_pt->clone());
}


void equations::NegativeExpression::get_info
    (equations::Visitor& pr) const {
    pr.visit(*this);
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


equations::Expression const* equations::DoubleVarExpression::get_left_exp_pt() const {
    return _left_exp;
}


equations::Expression const* equations::DoubleVarExpression::get_right_exp_pt() const {
    return _right_exp;
}


char equations::DoubleVarExpression::get_oper() const {
    return _oper;
}


equations::Expression* equations::DoubleVarExpression::clone() const {
    return new DoubleVarExpression(_left_exp->clone(), _right_exp->clone(), _oper);
}


void equations::DoubleVarExpression::get_info
    (equations::Visitor& pr) const {
    pr.visit(*this);
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


//Equation impl


equations::Equation::Equation
    (equations::Expression const* left_exp, equations::Expression const* right_exp) 
    : _left_exp(left_exp)
    , _right_exp(right_exp)
{
}


equations::Equation::~Equation() {
    delete _left_exp;
    delete _right_exp;
}


equations::Expression const* equations::Equation::get_left_exp() const {
    return _left_exp;
}


equations::Expression const* equations::Equation::get_right_exp() const {
    return _right_exp;
}


//solve impl
int equations::solve(equations::Equation const& eq) {
    equations::PrettySolver solver(eq);
    eq.get_left_exp()->get_info(solver);
    eq.get_right_exp()->get_info(solver);
    if (solver.failed()) {
        return 1;
    }
    solver.change_mode();
    IntVariable* operand = solver.get_var();
    if (operand) {
        int low_lim = operand->get_low_lim();
        int high_lim = operand->get_up_lim();
        int left_sum = 0;
        int right_sum = 0;
        for(int i = low_lim; i <= high_lim; ++i) {
            solver.set_num(i);
            eq.get_left_exp()->get_info(solver);
            left_sum = solver.get_sum();
            solver.clear_sum();
            if (!solver.failed()) {
                eq.get_right_exp()->get_info(solver);
                right_sum = solver.get_sum();
                solver.clear_sum();
                if (left_sum == right_sum && !solver.failed()) {
                    operand->set_value(i);
                    return 0;
                }
            }
            solver.clear_fail();
        }
    }
    return 1;
}


//ExpOutPut impl

equations::ExpOutPut::ExpOutPut(Expression const& exp, PrettyPrinter* pr)
    : _exp(exp)
    , _printer_pt(pr) 
{
}


equations::ExpOutPut::ExpOutPut(equations::ExpOutPut const& other)
    : _exp(other._exp) 
    , _printer_pt(other._printer_pt)
{
}


equations::Expression const& equations::ExpOutPut::get_exp_pt() const {
    return _exp;
}


equations::PrettyPrinter * equations::ExpOutPut::get_printer() const {
    return _printer_pt;
}


std::ostream& operator<<(std::ostream& out, equations::ExpOutPut const& exp_out) {
    exp_out.get_printer()->set_out(out);
    exp_out.get_exp_pt().get_info(*exp_out.get_printer());
    return out;
}


//PrettyPrinter impl

equations::PrettyPrinter::PrettyPrinter()
    : _out(nullptr) 
{
}

equations::ExpOutPut equations::PrettyPrinter::get_infix(equations::Expression const& exp) {
    return equations::ExpOutPut(exp, this);
} 

void equations::PrettyPrinter::set_out(std::ostream& out) {
    _out = &out;
}


void equations::PrettyPrinter::visit(equations::Expression const& exp) {
    exp.get_root_pt()->get_info(*this);
}


void equations::PrettyPrinter::visit(equations::VariableExpression const& exp) {
    *_out << exp.get_var()->show_name_var();
}


void equations::PrettyPrinter::visit(equations::LiteralExpression const& exp) {
    *_out << exp.get_num();
}


void equations::PrettyPrinter::visit(equations::NegativeExpression const& exp) {
    *_out << '-';
    if (exp.get_exp_pt()->priority() > exp.priority()) {
        *_out << '(';
    }
    exp.get_exp_pt()->get_info(*this);
    if (exp.get_exp_pt()->priority() > exp.priority()) {
        *_out << ')';
    }
}


void equations::PrettyPrinter::visit(equations::DoubleVarExpression const& exp) {
    if (exp.get_left_exp_pt()->priority() > exp.priority()) {
        *_out << '(';
    }
    exp.get_left_exp_pt()->get_info(*this);
    if (exp.get_left_exp_pt()->priority() > exp.priority()) {
       *_out << ')';
    }
    *_out << ' ' <<  exp.get_oper() << ' ';
    if (exp.get_right_exp_pt()->priority() >= exp.priority()) {
        *_out << '(';
    }
    exp.get_right_exp_pt()->get_info(*this);
    if (exp.get_right_exp_pt()->priority() >= exp.priority()) {
       *_out << ')';
    }
}


//PrettySolver impl
equations::PrettySolver::PrettySolver(equations::Equation const& eq)
    : _left_exp(eq.get_left_exp())
    , _right_exp(eq.get_right_exp())
    ,_var_pt(nullptr)
    ,sum(0)
    ,mode(TestMode)
    ,fail(false)
    ,num(0)
{
}


bool equations::PrettySolver::failed() const {
    return fail;
}


void equations::PrettySolver::change_mode() {
    mode = SolutionMode;
}


equations::IntVariable* equations::PrettySolver::get_var() {
    return _var_pt;
}

void equations::PrettySolver::set_num(int n) {
    num = n;
}

void equations::PrettySolver::clear_sum() {
    sum = 0;
}

void equations::PrettySolver::clear_fail() {
    fail = false;
}

int equations::PrettySolver::get_sum() const {
    return sum;
}

void equations::PrettySolver::visit(equations::Expression const& exp) {
    exp.get_root_pt()->get_info(*this);
}
void equations::PrettySolver::visit(equations::VariableExpression const& exp) {
    switch (mode)
    {
    case TestMode:
        if (_var_pt && _var_pt != exp.get_var()) {
            fail = true;
            break;
        } else if (!_var_pt) {
            _var_pt = exp.get_var();
        }
        break;
    case SolutionMode:
        sum = num;
        break;
    default:
        break;
    }
}
void equations::PrettySolver::visit(equations::LiteralExpression const& exp) {
    switch (mode)
    {
    case TestMode:
        break;
    case SolutionMode:
        sum = exp.get_num();
        break;
    default:
        break;
    }
}
void equations::PrettySolver::visit(equations::NegativeExpression const& exp) {
    switch (mode)
    {
    case TestMode:
        exp.get_exp_pt()->get_info(*this);
        break;
    case SolutionMode:
        exp.get_exp_pt()->get_info(*this);
        sum = -sum;
        break;
    default:
        break;
    }
}
void equations::PrettySolver::visit(equations::DoubleVarExpression const& exp) {
    switch (mode)
    {
    case TestMode:
        exp.get_left_exp_pt()->get_info(*this);
        exp.get_right_exp_pt()->get_info(*this);
        break;
    case SolutionMode:
        int left_sum;
        int right_sum;
        exp.get_left_exp_pt()->get_info(*this);
        left_sum = sum;
        sum = 0;
        exp.get_right_exp_pt()->get_info(*this);
        right_sum = sum;
        switch (exp.get_oper())
        {
        case '+':
            sum = left_sum + right_sum;
            break;
        case '-':
            sum = left_sum - right_sum;
            break;
        case '*':
            sum = left_sum * right_sum;
            break;
        case '/':
            if (right_sum == 0 || left_sum % right_sum != 0 ) {
                fail = true;
            } else {
                sum = left_sum/right_sum;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}


int main()
{
    equations::IntVariable x("x", -1, 1 ), y("y", -1, 1);
    equations::Expression exp1 = (3*x/3) ;
    equations::PrettyPrinter printer;
    if (equations::solve(exp1 == 1) == 0) {
        std::cout << x << std::endl;
    } else {
        std::cerr << "No solution" << std::endl;
    }
    std::cout << printer.get_infix(exp1) << std::endl;
    if (equations::solve(x + 3 == 100) == 0) {
        std::cout << x << std::endl;
    } else {
        std::cerr << "No solution" << std::endl;
    }
    if (equations::solve(x * x * x == 3 - (x + x)) == 0) {
        std::cout << x << std::endl;
    } else {
        std::cerr << "No solution" << std::endl;
    }
}



