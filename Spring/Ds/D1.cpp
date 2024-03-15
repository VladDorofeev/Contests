#include <iostream>

namespace equations
{
    class IntVariable;
    class Expression;
    class Equation;
    int solve(const Equation &eq);

    class IntVariable
    {
    public:
        IntVariable();

        int get_value() const;
        void set_value(int);
    private:
        int value;
    };
    IntVariable::IntVariable():value(0) {}
    std::ostream& operator << (std::ostream &os, const IntVariable &var) {
        return os << var.get_value();
    }
    int IntVariable::get_value() const {
        return value;
    }
    void IntVariable::set_value(int num) {
        value = num;
    }



    class Expression
    {
    public:
        Expression(int);
        Expression(IntVariable &);
        Expression(const Expression& exp) = default;
        Expression(bool is_good, bool just_num, int a, int b, IntVariable *var);

        bool good() const;
        void set_good(bool);

        bool is_num() const;
        void set_is_num(bool);

        int get_a() const;
        void set_a(int);

        int get_b() const;
        void set_b(int);

        IntVariable& get_var() const;
        void set_var(IntVariable *);
    private:
        bool is_good = true;
        bool just_num; 
        
        //a*var + b
        int a = 1;
        int b = 0;
        IntVariable *var = nullptr;
    };
    Expression::Expression(int num):just_num(true), a(0), b(num) {}
    Expression::Expression(IntVariable &x):just_num(false), var(&x) {}
    Expression::Expression(bool _is_good, bool _just_num, int _a, int _b, IntVariable *_var):
        is_good(_is_good), just_num(_just_num), a(_a), b(_b), var(_var) {}

    Expression operator+(const Expression &l, const Expression &r) {
        bool is_good = l.good() && r.good();
        bool just_num = l.is_num() && r.is_num();

        if ((!(l.is_num())) && (!(r.is_num())) 
            && (&l.get_var() != &r.get_var())){
            is_good = false;
        }

        IntVariable *var = nullptr;
        if (l.is_num()) {
            var = &r.get_var();
        } else {
            var = &l.get_var();
        }

        int a = l.get_a() + r.get_a();
        int b = l.get_b() + r.get_b();

        return Expression(is_good, just_num, a, b, var);
    }
    Expression operator*(const Expression &l, const Expression &r) {
        bool is_good = l.good() && r.good();
        bool just_num = l.is_num() && r.is_num();

        if ((!(l.is_num())) && (!(r.is_num()))) {
            is_good = false;
        }
        IntVariable *var = nullptr;
        if (l.is_num()) {
            var = &r.get_var();
        } else {
            var = &l.get_var();
        }

        //(ax+b)(cx+d) = acx^2 + (ad+bc)x + bd
        //(ax+b)(d) = adx + bd
        //(b)(cx+d) = cbx + bd
        int a = l.get_a()*r.get_b() + r.get_a()*l.get_b();
        int b = l.get_b() * r.get_b();

        return Expression(is_good, just_num, a, b, var);
    }
    Expression operator-(const Expression &exp) {
        Expression temp(exp);
        temp.set_a(-temp.get_a());
        temp.set_b(-temp.get_b());
        return temp;
    }
    Expression operator-(const Expression &l, const Expression &r) {
        return l + (-r);
    }

    bool Expression::good() const {return is_good;}
    void Expression::set_good(bool _is_good) {is_good = _is_good;}

    bool Expression::is_num() const {return just_num;}
    void Expression::set_is_num(bool _just_num) {just_num = _just_num;}

    int Expression::get_a() const {return a;}
    void Expression::set_a(int _a) {a = _a;}

    int Expression::get_b() const {return b;}
    void Expression::set_b(int _b) {b = _b;}

    IntVariable &Expression::get_var() const {return *var;}
    void Expression::set_var(IntVariable *_var) {var = _var;}

    class Equation
    {
    public:
        Equation(const Expression &, const Expression &);

        const Expression& get_l() const;
        const Expression& get_r() const;
    private:
        Expression left;
        Expression right;
    };
    Equation::Equation(const Expression &l, const Expression &r):left(l), right(r) {}
    Equation operator== (const Expression &l, const Expression &r) {
        return Equation(l,r);
    } 
    const Expression& Equation::get_l() const {return left;}
    const Expression& Equation::get_r() const {return right;}


    int solve(const Equation &eq) {
        //Check good both sides
        if (!((eq.get_l().good()) && (eq.get_r().good()))) {
            return 1;
        }
        //Check same variable in both sides
        if ((!(eq.get_l().is_num())) && (!(eq.get_r().is_num())) 
            && (&eq.get_l().get_var() != &eq.get_r().get_var())){
            return 1;
        }
        //Check that a != 0 in equation
        if (eq.get_l().get_a() - eq.get_r().get_a() == 0) {
            return 1;
        }
        //Check that result will be whole number
        if ((eq.get_r().get_b() - eq.get_l().get_b()) % (eq.get_l().get_a() - eq.get_r().get_a()) != 0) {
            return 1;
        }
        if (eq.get_l().is_num()) {
            eq.get_r().get_var().set_value((eq.get_l().get_b() - eq.get_r().get_b()) / 
                (eq.get_r().get_a() - eq.get_l().get_a()));
        } else {
            eq.get_l().get_var().set_value((eq.get_r().get_b() - eq.get_l().get_b()) / 
                (eq.get_l().get_a() - eq.get_r().get_a()));
        }
        return 0;        
    }

} 


#ifdef _main
int 
test(equations::Equation eq, int res) {
    if (equations::solve(eq) == 0) {
        return !(0 == res);
    } else {
        return !(1 == res);
    }
}

int main()
{
    equations::IntVariable x,y;

    std::cout << "Test #1 " << std::endl << (test(-x == 1, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #2 " << std::endl << (test(x + 1 == 123, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #3 " << std::endl << (test(2*x == 2, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #4 " << std::endl << (test(x*x - 1 == 2, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #5 " << std::endl << (test(x*x - x*x + x == 2, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #6 " << std::endl << (test(x*x*x*x*x + x == 123, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    
    //Mirror
    std::cout << "Test #7 " << std::endl << (test(1 == x, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #8 " << std::endl << (test(2 == x + 1, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #9 " << std::endl << (test(2 == 2*x, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #10 " << std::endl << (test(2 == x*x - 1, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #11 " << std::endl << (test(x*x - x*x + x == 1, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #12 " << std::endl << (test(123 == x*x*x*x*x + x, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;

    std::cout << "Test #13 " << std::endl << (test((x+123)*1 == 123, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #14 " << std::endl << (test((1+1)*x == 1232, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    
    std::cout << "Test #15 " << std::endl << (test(0*x == 1232, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #16 " << std::endl << (test(x+y-y == 1232, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #16 " << std::endl << (test(0*y + x == 1232, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #16 " << std::endl << (test((y-y) * x == 1232, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;
    std::cout << "Test #16 " << std::endl << (test((x-x) * x == 1232, 1) == 0 ? "passed " : "ERROR!!! ") << std::endl;

}
#endif
