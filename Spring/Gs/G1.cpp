#include <iostream>
#include <limits.h>
#include <stdexcept>

namespace no_exceptions
{

    enum ErrorCodes
    {
        SUCCESS,
        OVERFLOW,
        DIVBYZERO,
    };

    static const char *errors[] =
    {
        [SUCCESS] = "Success",
        [OVERFLOW] = "Integer overflow",
        [DIVBYZERO] = "Division by zero",
    };

    struct Expression
    {
        virtual int get_value(int &) const = 0;
        virtual ~Expression() = default;
    };
    
    struct IntVariable
    {
    public:
        IntVariable();
        void set_value(int);
        const int& get_value() const;
    private:
        int num;
    };

    struct VariableExpression : Expression
    {
    public:
        VariableExpression(const IntVariable &);
        int get_value(int &) const;
    private:
        const int &num;
    };

    struct LiteralExpression : Expression
    {
    public:
        LiteralExpression(int);
        int get_value(int &) const;
    private:
        int num;
    };
    
    struct BinaryExpression: Expression
    {
        Expression const & left;
        Expression const & right;
        BinaryExpression(Expression const &left_, Expression const &right_);
    };

    struct SumExpression: BinaryExpression
    {
        SumExpression(Expression const &left_, Expression const &right_);
        int get_value(int &) const;
    };

    struct MultExpression : BinaryExpression
    {
        MultExpression(Expression const &left_, Expression const &right_);
        int get_value(int &) const;
    };

    struct DivExpression : BinaryExpression
    {
        DivExpression(Expression const &left_, Expression const &right_);
        int get_value(int &) const;
    };

    IntVariable::IntVariable() : num(0) {}
    void IntVariable::set_value(int _num) {num = _num;}
    const int& IntVariable::get_value() const {return num;}

    VariableExpression::VariableExpression(const IntVariable &var) : num(var.get_value()) {}
    int VariableExpression::get_value(int &value) const {value = num; return SUCCESS;}

    LiteralExpression::LiteralExpression(int value) : num(value) {}
    int LiteralExpression::get_value(int &value) const {value = num; return SUCCESS;}

    BinaryExpression::BinaryExpression(Expression const &left_, Expression const &right_) :
        left(left_),
        right(right_)
    {
    }

    SumExpression::SumExpression(Expression const &left_, Expression const &right_) :
        BinaryExpression(left_, right_)
    {
    }

    int
    SumExpression::get_value(int &value) const
    {
        int ret;

        int left_value;
        if ((ret = left.get_value(left_value)) != 0) {
            return ret;
        }

        int right_value;
        if ((ret = right.get_value(right_value)) != 0) {
            return ret;
        }

        if ((left_value >= 0 && right_value > INT_MAX - left_value)
            || (left_value < 0 && right_value < INT_MIN - left_value)) {
            return OVERFLOW;
        }

        value = left_value + right_value;
        return SUCCESS;
    }

    MultExpression::MultExpression(Expression const &left_, Expression const &right_) :
        BinaryExpression(left_, right_)
    {
    }

    int MultExpression::get_value(int &value) const {
        int ret;

        int left_value;
        if ((ret = left.get_value(left_value)) != 0) {
            return ret;
        }

        int right_value;
        if ((ret = right.get_value(right_value)) != 0) {
            return ret;
        }

        if ((left_value >= 0 && right_value > INT_MAX / left_value)
            || (left_value < 0 && right_value < INT_MIN / left_value)) {
            return OVERFLOW;
        }

        value = left_value * right_value;
        return SUCCESS;
    }

    DivExpression::DivExpression(Expression const &left_, Expression const &right_) :
        BinaryExpression(left_, right_)
    {
    }

    int DivExpression::get_value(int &value) const {
        int ret;

        int left_value;
        if ((ret = left.get_value(left_value)) != 0) {
            return ret;
        }

        int right_value;
        if ((ret = right.get_value(right_value)) != 0) {
            return ret;
        }

        if (right_value == 0) {
            return DIVBYZERO;
        }
        
        if ((left_value == INT_MIN) && (right_value == -1)) {
            return OVERFLOW;
        }

        value = left_value / right_value;
        return SUCCESS;
    }
}

namespace with_exceptions
{
    enum ErrorCodes
    {
        SUCCESS,
        OVERFLOW,
        DIVBYZERO,
    };

    static const char *errors[] =
    {
        [SUCCESS] = "Success",
        [OVERFLOW] = "Integer overflow",
        [DIVBYZERO] = "Division by zero",
    };

    struct Expression
    {
        virtual int get_value() const = 0;
        virtual ~Expression() = default;
    };
    
    struct IntVariable
    {
    public:
        IntVariable();
        void set_value(int);
        const int& get_value() const;
    private:
        int num;
    };

    struct VariableExpression : Expression
    {
    public:
        VariableExpression(const IntVariable &);
        int get_value() const;
    private:
        const int &num;
    };

    struct LiteralExpression : Expression
    {
    public:
        LiteralExpression(int);
        int get_value() const;
    private:
        int num;
    };
    
    struct BinaryExpression: Expression
    {
        Expression const & left;
        Expression const & right;
        BinaryExpression(Expression const &left_, Expression const &right_);
    };

    struct SumExpression: BinaryExpression
    {
        SumExpression(Expression const &left_, Expression const &right_);
        int get_value() const;
    };

    struct MultExpression : BinaryExpression
    {
        MultExpression(Expression const &left_, Expression const &right_);
        int get_value() const;
    };

    struct DivExpression : BinaryExpression
    {
        DivExpression(Expression const &left_, Expression const &right_);
        int get_value() const;
    };

    IntVariable::IntVariable() : num(0) {}
    void IntVariable::set_value(int _num) {num = _num;}
    const int& IntVariable::get_value() const {return num;}

    VariableExpression::VariableExpression(const IntVariable &var) : num(var.get_value()) {}
    int VariableExpression::get_value() const {return num;}

    LiteralExpression::LiteralExpression(int value) : num(value) {}
    int LiteralExpression::get_value() const {return num;}

    BinaryExpression::BinaryExpression(Expression const &left_, Expression const &right_) :
        left(left_),
        right(right_)
    {
    }

    SumExpression::SumExpression(Expression const &left_, Expression const &right_) :
        BinaryExpression(left_, right_)
    {
    }

    int
    SumExpression::get_value() const
    {
        int left_value = left.get_value();

        int right_value = right.get_value();

        if ((left_value >= 0 && right_value > INT_MAX - left_value)
            || (left_value < 0 && right_value < INT_MIN - left_value)) {
            throw std::overflow_error(errors[OVERFLOW]);
        }

        return left_value + right_value;
    }

    MultExpression::MultExpression(Expression const &left_, Expression const &right_) :
        BinaryExpression(left_, right_)
    {
    }

    int MultExpression::get_value() const {
        int left_value = left.get_value();

        int right_value = right.get_value();

        if ((left_value >= 0 && right_value > INT_MAX / left_value)
            || (left_value < 0 && right_value < INT_MIN / left_value)) {
            throw std::overflow_error(errors[OVERFLOW]);
        }

        return left_value * right_value;
    }

    DivExpression::DivExpression(Expression const &left_, Expression const &right_) :
        BinaryExpression(left_, right_)
    {
    }

    int DivExpression::get_value() const {
        int left_value = left.get_value();

        int right_value = right.get_value();

        if (right_value == 0) {
            throw std::invalid_argument(errors[DIVBYZERO]);
        }
        
        if ((left_value == INT_MIN) && (right_value == -1)) {
            throw std::overflow_error(errors[OVERFLOW]);
        }

        return left_value / right_value;
    }
}


int main()
{
    {
        using namespace no_exceptions;
        IntVariable x, y, z;
        VariableExpression ex(x);
        VariableExpression ey(y);
        VariableExpression ez(z);
        LiteralExpression c(2);
        MultExpression e1(c, ey);//2 * 20
        SumExpression e2(ex, e1);//10 + 40
        DivExpression e3(ez, e2);//130 / 50
        x.set_value(10);
        y.set_value(20);
        z.set_value(130);
        int value, ret;
        if ((ret = e3.get_value(value)) == 0) {
            std::cout << value << std::endl;
        } else {
            std::cout << errors[ret] << std::endl;
        }
    }
    {
        using namespace with_exceptions;
        IntVariable x, y, z;
        VariableExpression ex(x);
        VariableExpression ey(y);
        VariableExpression ez(z);
        LiteralExpression c(2);
        MultExpression e1(c, ey);//2 * 20
        SumExpression e2(ex, e1);//10 + 40
        DivExpression e3(ez, e2);//130 / 50
        x.set_value(10);
        y.set_value(20);
        z.set_value(130);
        try
        {
            std::cout << e3.get_value() << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
        
        
        //Bad Expressions
        x.set_value(INT_MAX);
        LiteralExpression some_const(86);
        SumExpression sumerr(some_const, ex);
        try
        {
            std::cout << sumerr.get_value() << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }

        x.set_value(0);
        DivExpression diverr(some_const, ex);
        try
        {
            std::cout << diverr.get_value() << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }

    }
}