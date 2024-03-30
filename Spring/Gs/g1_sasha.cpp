#include <iostream>
#include <limits.h>


namespace no_exceptions
{

    enum ErrorCodes
    {
        SUCCESS,
        OVERFLOWING,
        DIVBYZERO,
    };

    static const char *errors[] =
    {
        [SUCCESS] = "Success",
        [OVERFLOWING] = "Integer overflow",
        [DIVBYZERO] = "Division by zero",
    };

    class IntVariable {
    public:
        IntVariable();
        void set_value(int);
        int get_value() const;
    private:
        int val;
    };


    IntVariable::IntVariable()
        : val(0) 
    {
    }


    void IntVariable::set_value(int num) {
        val = num;
    }

    int IntVariable::get_value() const {
        return val;
    }


    struct Expression
    {
        virtual int get_value(int&) const = 0;
        virtual ~Expression() = default;
    };


    class VariableExpression : public Expression {
    public:
        VariableExpression(IntVariable&);
        int get_value(int&) const;
    private:
        IntVariable& var;
    };


    VariableExpression::VariableExpression(IntVariable& _var)
        : var(_var)
    {
    }

    int VariableExpression::get_value(int& value) const {
        value = var.get_value();
        return SUCCESS;
    }


    class LiteralExpression : public Expression {
    public:
        LiteralExpression(int);
        int get_value(int&) const;
    private:
        int val;
    };


    LiteralExpression::LiteralExpression(int num)
        : val(num) 
    {
    }

    int LiteralExpression::get_value(int& value) const {
        value = val;
        return SUCCESS;
    }


    struct BinaryExpression: Expression
    {
        Expression const & left;
        Expression const & right;
        BinaryExpression(Expression const &left_, Expression const &right_);
    };

    BinaryExpression::BinaryExpression(Expression const& left_, Expression const& right_)
        : left(left_)
        , right(right_)
    {
    }

    struct SumExpression: BinaryExpression
    {
        SumExpression(Expression const &left_, Expression const &right_);
        int get_value(int&) const;
    };

    SumExpression::SumExpression(Expression const& left_, Expression const& right_)
        : BinaryExpression(left_, right_) 
    {
    } 

    int
    SumExpression::get_value(int& value) const
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
            return OVERFLOWING;
        }

        value = left_value + right_value;
        return SUCCESS;
    } 

    class MultExpression : public BinaryExpression {
    public:
        MultExpression(Expression const &left_, Expression const &right_);
        int get_value(int&) const;
    };

    MultExpression::MultExpression(Expression const &left_, Expression const &right_) 
        : BinaryExpression(left_, right_)
    {
    }
        
    int
    MultExpression::get_value(int& value) const
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
        
        if ((left_value != 0 && right_value != 0) 
            && ((left_value > 0 && ((right_value > 0 
            && right_value > INT_MAX / left_value)
            || (right_value < 0 && left_value > INT_MIN / right_value))) 
            || (left_value < 0 && ((right_value > 0 
            && left_value < INT_MIN / right_value)
            || (right_value < 0 && left_value < INT_MAX / right_value))))) {
            return OVERFLOWING;
        }

        value = left_value * right_value;
        return SUCCESS;
    }


    class DivExpression : public BinaryExpression {
    public:
        DivExpression(Expression const &left_, Expression const &right_);
        int get_value(int&) const;
    };

    DivExpression::DivExpression(Expression const &left_, Expression const &right_) 
        : BinaryExpression(left_, right_)
    {
    }
        
    int
    DivExpression::get_value(int& value) const
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
        
        if (right_value == 0) {
            return DIVBYZERO;
        }
        if (left_value == INT_MIN && right_value == -1) {
            return OVERFLOWING;
        }

        value = left_value / right_value;
        return SUCCESS;
    }
}

namespace with_exceptions
{

    class IntVariable {
    public:
        IntVariable();
        void set_value(int);
        int get_value() const;
    private:
        int val;
    };


    IntVariable::IntVariable()
        : val(0) 
    {
    }


    void IntVariable::set_value(int num) {
        val = num;
    }

    int IntVariable::get_value() const {
        return val;
    }


    struct Expression
    {
        virtual int get_value() const = 0;
        virtual ~Expression() = default;
    };


    class VariableExpression : public Expression {
    public:
        VariableExpression(IntVariable&);
        int get_value() const;
    private:
        IntVariable& var;
    };


    VariableExpression::VariableExpression(IntVariable& _var)
        : var(_var)
    {
    }

    int VariableExpression::get_value() const {
        return var.get_value();
    }


    class LiteralExpression : public Expression {
    public:
        LiteralExpression(int);
        int get_value() const;
    private:
        int val;
    };


    LiteralExpression::LiteralExpression(int num)
        : val(num) 
    {
    }

    int LiteralExpression::get_value() const {
        return val;
    }


    struct BinaryExpression: Expression
    {
        Expression const & left;
        Expression const & right;
        BinaryExpression(Expression const &left_, Expression const &right_);
    };

    BinaryExpression::BinaryExpression(Expression const& left_, Expression const& right_)
        : left(left_)
        , right(right_)
    {
    }

    struct SumExpression: BinaryExpression
    {
        SumExpression(Expression const &left_, Expression const &right_);
        int get_value() const;
    };

    SumExpression::SumExpression(Expression const& left_, Expression const& right_)
        : BinaryExpression(left_, right_) 
    {
    } 

    int
    SumExpression::get_value() const
    {
        int left_value = left.get_value();
        int right_value = right.get_value();
        if ((left_value >= 0 && right_value > INT_MAX - left_value)
            || (left_value < 0 && right_value < INT_MIN - left_value)) {
            throw std::overflow_error("Integer overflow");
        }
        return left_value + right_value;
    } 

    class MultExpression : public BinaryExpression {
    public:
        MultExpression(Expression const &left_, Expression const &right_);
        int get_value() const;
    };

    MultExpression::MultExpression(Expression const &left_, Expression const &right_) 
        : BinaryExpression(left_, right_)
    {
    }
        
    int
    MultExpression::get_value() const
    {
        int left_value = left.get_value();
        int right_value = right.get_value();
        if ((left_value != 0 && right_value != 0) 
            && ((left_value > 0 && ((right_value > 0 
            && right_value > INT_MAX / left_value)
            || (right_value < 0 && left_value > INT_MIN / right_value))) 
            || (left_value < 0 && ((right_value > 0 
            && left_value < INT_MIN / right_value)
            || (right_value < 0 && left_value < INT_MAX / right_value))))) {
            throw std::overflow_error("Integer overflow");
        }
        return left_value * right_value;
    }


    class DivExpression : public BinaryExpression {
    public:
        DivExpression(Expression const &left_, Expression const &right_);
        int get_value() const;
    };

    DivExpression::DivExpression(Expression const &left_, Expression const &right_) 
        : BinaryExpression(left_, right_)
    {
    }
        
    int
    DivExpression::get_value() const
    {
        int left_value = left.get_value();
        int right_value = right.get_value();
        if (right_value == 0) {
            throw std::logic_error("Division by zero");
        }
        if (left_value == INT_MIN && right_value == -1) {
            throw std::overflow_error("Integer overflow");
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
        MultExpression e1(c, ey);
        SumExpression e2(ex, e1);
        DivExpression e3(ez, e2);
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
        // TODO
        IntVariable x, y, z;
        VariableExpression ex(x);
        VariableExpression ey(y);
        VariableExpression ez(z);
        LiteralExpression c(2);
        MultExpression e1(c, ey);
        SumExpression e2(ex, e1);
        DivExpression e3(ez, e2);
        x.set_value(10);
        y.set_value(20);
        z.set_value(130);
        int value;
        try {
            value = e3.get_value();
            std::cout << value << std::endl;
        } catch(std::exception& err) {
            std::cout << err.what() << std::endl;
        }
        x.set_value(1);
        y.set_value(INT_MAX);
        SumExpression e4(ex, ey);
        try {
            value = e4.get_value();
            std::cout << value << std::endl;
        } catch (std::exception& err) {
            std::cout << err.what() << std::endl;
        }
        x.set_value(0);
        y.set_value(0);
        DivExpression e5(ex, ey);
        try {
            value = e5.get_value();
            std::cout << value << std::endl;
        } catch (std::exception& err) {
            std::cout << err.what() << std::endl;
        }
    }
}
