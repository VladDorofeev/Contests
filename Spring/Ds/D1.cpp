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
        friend std::ostream& operator << (std::ostream &os, const IntVariable &var);
        friend Expression;
        friend Equation;
        friend int solve(const Equation &);

        Expression operator-();
    private:
        int value;
    };
    IntVariable::IntVariable():value(0) {}
    std::ostream& operator << (std::ostream &os, const IntVariable &var) {
        return os << var.value;
    }



    class Expression
    {
    public:
        Expression();
        Expression(int);
        Expression(IntVariable &);
        Expression(const Expression& exp) = default;

        friend Equation;
        friend int solve(const Equation &);
        friend Expression operator+(const Expression&, const Expression&);
        friend Expression operator*(const Expression&, const Expression&);
        friend Expression operator-(const Expression&, const Expression&);
        Expression operator-() const;

        void print()const;
    private:
        bool is_good = true;
        bool just_num; 
        
        //a*var + b
        int a = 1;
        int b = 0;
        IntVariable *var = nullptr;
    };
    Expression::Expression(): is_good(false), just_num(false) {}
    Expression::Expression(int num):just_num(true), a(0), b(num) {}
    Expression::Expression(IntVariable &x):just_num(false), var(&x) {}
    void Expression::print()const {
        std::cout << "a=" << a << " b=" << b << " is_good=" << is_good;
        std::cout << " just_num=" << just_num << " var=" << var << std::endl;
    }
    Expression operator+(const Expression &l, const Expression &r) {
        Expression exp;
        exp.is_good = l.is_good && r.is_good;
        exp.just_num = l.just_num && r.just_num;

        if ((!(l.just_num)) && (!(r.just_num)) 
            && (l.var != r.var)){
            exp.is_good = false;
        }

        if (l.just_num) {
            exp.var = r.var;
        } else {
            exp.var = l.var;
        }

        exp.a = l.a + r.a;
        exp.b = l.b + r.b;

        return exp;
    }
    Expression operator*(const Expression &l, const Expression &r) {
        Expression exp; 
        exp.is_good = l.is_good && r.is_good;
        exp.just_num = l.just_num && r.just_num;

        if ((!(l.just_num)) && (!(r.just_num))) {
            exp.is_good = false;
        }

        if (l.just_num) {
            exp.var = r.var;
        } else {
            exp.var = l.var;
        }

        //(ax+b)(cx+d) = acx^2 + (ad+bc)x + bd
        //(ax+b)(d) = adx + bd
        //(b)(cx+d) = cbx + bd
        exp.a = l.a*r.b + r.a*l.b;
        exp.b = l.b * r.b;

        return exp;
    }
    Expression Expression::operator-() const {
        Expression temp(*this);
        temp.a = -a;
        temp.b = -b;
        return temp;
    }
    Expression operator-(const Expression &l, const Expression &r) {
        return l + (-r);
    }


    class Equation
    {
    public:
        Equation(const Expression &, const Expression &);
        friend Equation operator== (const Expression &, const Expression &); 
        friend int solve(const Equation&);
    private:
        Expression left;
        Expression right;
    };
    Equation::Equation(const Expression &l, const Expression &r):left(l), right(r) {}
    Equation operator== (const Expression &l, const Expression &r) {
        Equation eq(l,r);
        return eq;
    } 


    int solve(const Equation &eq) {
        //Check good both sides
        if (!((eq.left.is_good) && (eq.right.is_good))) {
            return 1;
        }
        //Check same variable in both sides
        if ((!(eq.left.just_num)) && (!(eq.right.just_num)) 
            && (eq.left.var != eq.right.var)){
            return 1;
        }
        //Check that a != 0 in equation
        if (eq.left.a - eq.right.a == 0) {
            return 1;
        }
        //Check that result will be whole number
        if ((eq.right.b - eq.left.b) % (eq.left.a - eq.right.a) != 0) {
            return 1;
        }
        if (eq.left.just_num) {
            eq.right.var->value = (eq.left.b - eq.right.b) / (eq.right.a - eq.left.a);
        } else {
            eq.left.var->value = (eq.right.b - eq.left.b) / (eq.left.a - eq.right.a);
        }
        return 0;        
    }

    Expression IntVariable::operator-() {
        return -(Expression(*this));
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

    std::cout << "Test #1 " << std::endl << (test(x == 1, 0) == 0 ? "passed " : "ERROR!!! ") << std::endl;
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
