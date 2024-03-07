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
        Expression &operator-();
        
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
    Expression 
    Expression::operator-() const {
        Expression temp(*this);
        temp.a = -a;
        temp.b = -b;
        return temp;
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
        l.print();
        r.print();
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

        eq.left.var->value = (eq.right.b - eq.left.b) / (eq.left.a - eq.right.a);
        return 0;        
    }

} 


#ifdef _main
int 
test(equations::Equation eq) {
    if (equations::solve(eq) == 0) {
        return 0;
    } else {
        return 1;
    }
}

int main()
{
    equations::IntVariable x,y;

    std::cout << "Test #1 " << std::endl << (test(2*x+4 ==-x + 7) == 0 ? "passed, res = " : "No sol!!! res = ") << x << std::endl;
}
#endif


/*
void IntVariable::print() const {
        std::cout << a << 'x' << '+' << b << " , " << "is_g=" << is_good;
        std::cout << " num= " << just_num << " ptr="  << ptr << std::endl; 
    }

    IntVariable::IntVariable():just_num(false), ptr(this) {
    }
    IntVariable::IntVariable(int num):a(0), b(num), just_num(true), ptr(nullptr) {
    }
    IntVariable::IntVariable(const IntVariable& var) {
        a = var.a;
        b = var.b;
        x = var.x;
        is_good = var.is_good;
        just_num = var.just_num;
        ptr = var.ptr;
    }
    IntVariable::IntVariable(IntVariable&& var) {
        a = var.a;
        b = var.b;
        x = var.x;
        is_good = var.is_good;
        just_num = var.just_num;
        ptr = var.ptr;
    }
    IntVariable IntVariable::operator-() const{
        IntVariable temp(*this);
        temp.a = -a;
        temp.b = -b;
        return temp;
    }

    class Equation
    {
    public:
        Equation(IntVariable *a);

        friend Equation operator== (IntVariable a, IntVariable b); 
        friend int solve(const Equation& eq);
    private:
        //ax + b == 0
        bool is_good;
        IntVariable *x;
    };

    Equation::Equation(IntVariable *a): is_good(false), x(a) {}

    std::ostream& operator << (std::ostream &os, const equations::IntVariable &var) {
        return os << var.x;
    }

    IntVariable operator+ (const IntVariable& a, const IntVariable& b) {
        IntVariable temp;
        temp.is_good = a.is_good && b.is_good;
        
        //Checking pointers (only one variable in equation)
        if (!( ((!a.just_num) && (!b.just_num) && (a.ptr == b.ptr)) 
            || (a.just_num) || (b.just_num) )){
            temp.is_good = false;

            return temp;
        }

        //We need understand what is variable
        if (a.just_num) {
            temp.ptr = b.ptr;
        } else {
            temp.ptr = a.ptr;
        }
        
        
        temp.a = a.a + b.a;
        temp.b = a.b + b.b;

        return temp;
    }
    IntVariable operator- (const IntVariable& a, const IntVariable& b) {
        IntVariable temp = a + (-b);

        return temp;
    }
    
    IntVariable operator* (const IntVariable& a, const IntVariable& b) {
        IntVariable temp;
        temp.is_good = a.is_good && b.is_good;
        
        //Check x*x (x*y / y*x)
        if ((!a.just_num) && (!b.just_num)) {
            temp.is_good = false;

            return temp;
        }

        //We need understand what is variable
        if (a.just_num) {
            temp.ptr = b.ptr;
        } else {
            temp.ptr = a.ptr;
        }
        

        temp.a = a.a*b.b + b.a*a.b;
        temp.b = a.b * b.b;

        return temp;
    }

    class IntVariable
    {
    public:
        IntVariable();
        IntVariable(int num);
        IntVariable(const IntVariable&);
        IntVariable(IntVariable&&);

        friend Equation;
        friend std::ostream& operator << (std::ostream &os, const IntVariable &var);
        friend IntVariable operator+ (const IntVariable& a, const IntVariable& b);
        friend IntVariable operator* (const IntVariable& a, const IntVariable& b);
        
        friend Equation operator== (IntVariable a, IntVariable b); 

        IntVariable operator-()const;

        friend int solve(const Equation &eq);

        void print()const;
    private:
        //ax + b
        int a = 1;
        int b = 0;
        int x = 0;
        bool is_good = true;
        bool just_num;
        IntVariable *ptr;
    };
*/