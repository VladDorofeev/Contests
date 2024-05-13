
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <algorithm>

/*
    Грамматика:

    Для лучшего понимания нетерминалы обозначены не одной заглавной буквой, 
    а Словом (между словами для читаемости допускается пробел)
    
    Чистая без действий: 

        S          ->     Conjunc
        Conjunc    ->     Disjunc {| Disjunc}
        Disjunc    ->     Compare {& Compare}
        Compare    ->     Plus    {<>= Plus}
        Plus       ->     Mult    {+ Mult}
        Mult       ->     Operand {* Operand}
        Operand    ->     'T' | 'F' | digit | '(' S ')'
*/

class Parser
{
public:
    Parser() = default;
    void parse(std::istream &);
private:
    int c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void Mult();
    void Plus();
    void Compare();
    void Disjunc();
    void Conjunc();
    void Operand();

    /* < */
    enum Type{BOOL, INT};
    std::stack<Type> stack;
    /* > */

    const char * print_type(Type arg) {return (arg==BOOL?"BOOL":"INT");}
};

void Parser::gc() {
    c = in->get();
}

void Parser::parse(std::istream &_in) {
    /* < */
    stack = std::stack<Type>();
    /* > */

    in = &_in;
    gc();
    S();

    if (c != EOF) {
        throw std::logic_error("Not a EOF");
    }
}

void Parser::S() {
    Conjunc();
}

void Parser::Conjunc() {
    Disjunc();

    while (c == '|') {
        gc();
        Disjunc();

        Type op2 = stack.top();
        stack.pop();

        Type op1 = stack.top();
        stack.pop();
        std::cout << "parse |, op1 = " << print_type(op1) << " op2 = " << print_type(op2) << std::endl; 
        if ((op1 == op2) && (op1 == BOOL)) {
            stack.push(BOOL);
        } else {
            throw std::logic_error("Dismatch types in |");
        }
    }
}

void Parser::Disjunc() {
    Compare();

    while (c == '&') {
        gc();
        Compare();

        Type op2 = stack.top();
        stack.pop();

        Type op1 = stack.top();
        stack.pop();
        std::cout << "parse &, op1 = " << print_type(op1) << " op2 = " << print_type(op2) << std::endl; 
        if ((op1 == op2) && (op1 == BOOL)) {
            stack.push(BOOL);
        } else {
            throw std::logic_error("Dismatch types in &");
        }
    }
}

void Parser::Compare() {
    Plus();

    while ((c == '>') || (c == '<') || (c == '=')) {
        gc();
        Plus();

        Type op2 = stack.top();
        stack.pop();

        Type op1 = stack.top();
        stack.pop();
        std::cout << "parse <>=, op1 = " << print_type(op1) << " op2 = " << print_type(op2) << std::endl; 
        if ((op1 == op2) && (op1 == INT)) {
            stack.push(BOOL);
        } else {
            throw std::logic_error("Dismatch types in <>=");
        }
    }
}

void Parser::Plus() {
    Mult();

    while (c == '+') {
        gc();
        Mult();

        Type op2 = stack.top();
        stack.pop();

        Type op1 = stack.top();
        stack.pop();
        std::cout << "parse +, op1 = " << print_type(op1) << " op2 = " << print_type(op2) << std::endl; 
        if ((op1 == op2) && (op1 == INT)) {
            stack.push(INT);
        } else {
            throw std::logic_error("Dismatch types in +");
        }
    }
}

void Parser::Mult() {
    Operand();

    while (c == '*') {
        gc();
        Operand();

        Type op2 = stack.top();
        stack.pop();

        Type op1 = stack.top();
        stack.pop();
        std::cout << "parse *, op1 = " << print_type(op1) << " op2 = " << print_type(op2) << std::endl; 
        if ((op1 == op2) && (op1 == INT)) {
            stack.push(INT);
        } else {
            throw std::logic_error("Dismatch types in *");
        }
    }
}









void Parser::Operand() {
    if (c == 'T') {
        std::cout << "pushed T" << std::endl;
        stack.push(BOOL);
    } else if (c == 'F') {
        std::cout << "pushed F" << std::endl;
        stack.push(BOOL);
    } else if (std::isdigit(c)) {
        std::cout << "pushed " << c - '0' << std::endl;
        stack.push(INT);
    } else if (c == '(') {
        gc();
        S();
        if (c != ')') {
            throw std::logic_error("Not enough '('");
        }
    } else {
        throw std::logic_error("Bad char");
    }
    gc();
}

int main() {
    Parser parser;
    
    std::string line;
    
    while (std::getline(std::cin, line)) {
        std::cout << "------------------" << std::endl;
        std::cout << line << std::endl;
        std::stringstream input(line);
        try
        {
            parser.parse(input);
            std::cout << "YES" << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            // std::cout << "NO" << std::endl;
        }
        std::cout << "------------------" << std::endl;
    }
    return 0;
}
