#include <iostream>

namespace equations
{
    class IntVariable;
    class Equation;
    int solve(const Equation &eq);

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

        IntVariable operator-()const;
        friend Equation operator== (IntVariable a, IntVariable b); 
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
        
        //(ax+b)(cx+d) = acx^2 + (ad+bc)x + bd
        //(ax+b)(d) = adx + bd
        //(b)(cx+d) = cbx + bd
        temp.a = a.a*b.b + b.a*a.b;
        temp.b = a.b * b.b;

        return temp;
    }


    Equation operator== (IntVariable a, IntVariable b) {
        Equation eq(a.ptr);
        if (a.just_num) {
            eq.x = b.ptr;
        }
        if (a.is_good && b.is_good) {
            if ( ((!a.just_num) && (!b.just_num) && (a.ptr == b.ptr)) 
            || (a.just_num) || (b.just_num) ){

                eq.is_good = true;
                
                eq.x->a = a.a - b.a;
                eq.x->b = a.b - b.b;
            }
        }
        return eq;
    }


    int solve(const Equation &eq) {
        //Check status variable
        if (!(eq.is_good)) {
            return 1;
        }

        //Check koef a not equal zero (ax+b)
        if (eq.x->a == 0) {
            return 1;
        }

        //Check is int result
        if ((-eq.x->b % eq.x->a) != 0) {
            return 1;
        }

        eq.x->x = -eq.x->b / eq.x->a;

        return 0;
    }



} 

