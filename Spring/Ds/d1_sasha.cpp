#include <iostream>

namespace equations {
    class IntVariable {
    public:
        IntVariable();
        int get_val() const;
        void set_val(int);
    private:
        int val;
    };

    class Expression {
    public:
        Expression();
        Expression(int);
        Expression(IntVariable&);
        Expression(const Expression&);
        int get_val() const;
        int get_kf() const;
        bool failed() const;
        void set_val(int);
        void set_kf(int);
        void set_fail(bool);
        void set_var_pt(IntVariable*);
        IntVariable* get_var_pt() const;
    private:
        int val;
        int kf;
        bool fail;
        IntVariable* orig_var_pt;
    };

    class Equation {
    public:
        Equation(const Expression&);
        bool can_set_root() const;
    private:
        Expression eq_exp;
    };
    int solve(const Equation&);
}

//IntVariable def
equations::IntVariable::IntVariable(): val(0) {
}

int equations::IntVariable::get_val() const {
    return val;
}
void equations::IntVariable::set_val(int num) {
    val = num;
}

std::ostream& operator<<(std::ostream& out, equations::IntVariable var) {
    return out<<var.get_val();
}

//Expression def
equations::Expression::Expression()
    : val(0), kf(0), fail(false), orig_var_pt(nullptr) {
}
equations::Expression::Expression(int num)
    : val(num), kf(0), fail(false), orig_var_pt(nullptr){
}
equations::Expression::Expression(equations::IntVariable& var)
    : val(0), kf(1), fail(false), orig_var_pt(&var) {
}
equations::Expression::Expression(const equations::Expression& other)
    : val(other.val), kf(other.kf), fail(other.fail), orig_var_pt(other.orig_var_pt) {
}

int equations::Expression::get_val() const{
    return val;
}
int equations::Expression::get_kf() const {
    return kf;
}
bool equations::Expression::failed() const {
    return fail;
}
equations::IntVariable* equations::Expression::get_var_pt() const{
    return orig_var_pt;
}
void equations::Expression::set_val(int num) {
    val = num;
}
void equations::Expression::set_kf(int num) {
    kf = num;
}
void equations::Expression::set_fail(bool status) {
    fail = status;
}
void equations::Expression::set_var_pt(equations::IntVariable* pt) {
    orig_var_pt = pt;
}

equations::Expression operator+
    (const equations::Expression& exp1,const equations::Expression& exp2) {
    equations::Expression new_exp;
    if ((exp1.failed() || exp2.failed()) 
        || (exp1.get_var_pt() && exp2.get_var_pt()
        && exp1.get_var_pt() != exp2.get_var_pt())) {
        new_exp.set_fail(true);
        return new_exp;
    }
    new_exp.set_val(exp1.get_val() + exp2.get_val());
    new_exp.set_kf(exp1.get_kf() + exp2.get_kf());
    new_exp.set_var_pt(exp1.get_var_pt()?exp1.get_var_pt():exp2.get_var_pt());
    return new_exp;
}
equations::Expression operator-(const equations::Expression& exp) {
    equations::Expression new_exp(exp);
    new_exp.set_val(-exp.get_val());
    new_exp.set_kf(-exp.get_kf());
    return new_exp;
}
equations::Expression operator-
    (const equations::Expression& exp1, const equations::Expression& exp2) {
    return exp1 + (-exp2);
}
equations::Expression operator*
    (const equations::Expression& exp1, const equations::Expression& exp2) {
    equations::Expression new_exp;
    if ((exp1.failed() || exp2.failed()) 
        || (exp1.get_var_pt() && exp2.get_var_pt())) {
        new_exp.set_fail(true);
        return new_exp;
    }
    new_exp.set_val(exp1.get_val() * exp2.get_val());
    new_exp.set_kf(exp1.get_var_pt()?exp1.get_kf()*exp2.get_val():exp1.get_val()*exp2.get_kf());
    new_exp.set_var_pt(exp1.get_var_pt()?exp1.get_var_pt():exp2.get_var_pt());
    return new_exp;
}
equations::Equation operator==
    (const equations::Expression& exp1, const equations::Expression& exp2) {
        return exp1 - exp2;
    }

//Equation def
equations::Equation::Equation(const equations::Expression& exp)
: eq_exp(exp) {
}
bool equations::Equation::can_set_root() const{
    if (eq_exp.failed() || (eq_exp.get_kf() == 0) || (eq_exp.get_val()%eq_exp.get_kf() != 0)) {
        return false;
    }
    //set_root
    eq_exp.get_var_pt()->set_val(-eq_exp.get_val()/eq_exp.get_kf());
    return true;
}
//solve
int equations::solve(const equations::Equation& eq) {
    if (eq.can_set_root()) {
        return 0;
    }
    return 1;
}

/*int
main() {
    equations::IntVariable x;
    equations::IntVariable y;
    if (equations::solve(x + 1 == 24) == 0) {
        std::cout << x << std::endl;
    } else {
        std::cerr << "No solution" << std::endl;
    }
}*/