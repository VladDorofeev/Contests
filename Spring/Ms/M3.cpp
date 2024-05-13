
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

        S -> Assign Comma
        Comma -> ',' S | eps
        Assign -> Arithm Equal
        Equal -> '=' Assign | eps
        Arithm -> Op Sub
        Sub -> '-' Op Sub | eps
        Op -> Var | Num | '(' S ')'
        Var -> буква
        Num -> цифра 


    С действиями:

        S       ->    <...> Assign Comma <...>
        Comma   ->    ',' <...> S | eps
        Assign  ->    Arithm Equal
        Equal   ->    '=' <...> Assign <...> | <...> eps
        Arithm  ->    Op Sub
        Sub     ->    '-' Op <...> Sub <...> | eps
        Op      ->    Var | Num | '(' S ')'
        Var     ->    буква <...>
        Num     ->    цифра <...>

*/

class Parser
{
public:
    Parser() = default;
    void parse(std::istream &);

    int get_value() const;
private:
    int c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void Assign();
    void Comma();
    void Arithm();
    void Equal();
    void Op();
    void Sub();
    void Var();
    void Num();

    bool lvalue = false;
    int value = 0;
    std::map<int, int> vars;
    std::stack<int> stack;

    int get_value_of_var(int);
};

void Parser::gc() {
    c = in->get();
}

void Parser::parse(std::istream &_in) {
    /* < */
    lvalue = false;
    vars.clear();
    stack = std::stack<int>();
    /* > */

    in = &_in;
    gc();
    S();

    if (c != EOF) {
        throw std::logic_error("Not a EOF");
    }

    /* < */
    value = stack.top();
    stack.pop();
    /* > */

}

void Parser::S() {
    Assign(); 
    Comma();
}

void Parser::Comma() {
    if (c == ',') {

        /* < */
        lvalue = false;
        stack.pop();
        /* > */

        gc();
        S();
    }
}

void Parser::Assign() {
    Arithm();
    Equal();
}

void Parser::Equal() {
    if (c == '=') {

        /* < */
        if (!lvalue) {
            throw std::logic_error("Not a lvalue in equal");
        }
        /* > */

        gc();
        Assign();
    
        /* < */
        int op = stack.top();
        stack.pop();

        if (lvalue) {
            op = get_value_of_var(op);
        }

        int var = stack.top();
        stack.pop();

        vars[var] = op;
        stack.push(op);

        lvalue = false;
        /* > */

    } else {
        /* < */
        int op = stack.top();
        stack.pop();
        if (lvalue) {
            op = get_value_of_var(op);
        }
        stack.push(op);
        /* < */
    }
}

void Parser::Arithm() {
    Op();
    Sub();
}

void Parser::Sub() {
    if (c == '-') {
        /* < */
        bool saved_lvalue = lvalue;
        lvalue = false;
        /* > */
        
        gc();
        Op();

        /* < */
        int op2 = stack.top();
        stack.pop();
        int op1 = stack.top();
        stack.pop();

        //check that operand is var
        if (saved_lvalue) {
            op1 = get_value_of_var(op1);
        }
        if (lvalue) {
            op2 = get_value_of_var(op2);
        }

        stack.push(op1 - op2);

        lvalue = false;
        /* > */

        Sub();
    }
}

void Parser::Op() {
    if (std::isalpha(c)) {
        Var();
    } else if (std::isdigit(c)) {
        Num();
    } else if (c == '(') {
        gc();
        S();
        if (c != ')') {
            throw std::logic_error("Not enough )");
        } else {
            gc();
        }
    } else {
        throw std::logic_error("Bad argument");
    }
}

void Parser::Var() {
    /* < */
    lvalue = true;
    stack.push(c);
    /* > */

    gc();
}

void Parser::Num() {
    /* < */
    lvalue = false;
    stack.push(c - '0');
    /* > */

    gc();
}

int Parser::get_value() const { return value; }

int Parser::get_value_of_var(int operand) {
    if (vars.find(operand) == vars.end()) {
        throw std::logic_error("Not init var");
    } else {
        return vars[operand];
    }
}


int main() {
    Parser parser;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);
        try
        {
            parser.parse(input);
            std::cout << parser.get_value() << std::endl;
        }
        catch(const std::exception &)
        {
            std::cout << "NO" << std::endl;
        }
    }
    return 0;
}
