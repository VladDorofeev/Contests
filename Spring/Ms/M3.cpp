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

        <init helpful vars>
        S -> Assign Comma
        Comma -> ',' <clear vars> S | eps
        Assign -> Arithm Equal
        Equal -> '=' <if not lvalue then ERROR> Assign <assign> | eps
        Arithm -> Op Sub
        Sub -> '-' Op <calculate result, push> Sub | eps
        Op -> Var | Num | '(' S ')'
        Var -> буква <lvalue=true, stack.push(var)>
        Num -> цифра <lvalue=false, stack.push(num)>

*/

class Parser
{
public:
    Parser() = default;
    void parse(std::istream &);

    int get_value() const;
    void print_vars() {std::for_each(vars.begin(), vars.end(), [](auto p) {std::cout << static_cast<char>(p.first) << ':' << p.second << std::endl;});}
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
        std::cout << "EOF on " << static_cast<char> (c) << " ";
        throw c;
    }

    value = stack.top();
    stack.pop();
    if (lvalue) {
        value = get_value_of_var(value);
    }

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
            std::cout << "not lvalue ";
            throw c;
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
            std::cout << ") ";
            throw c;
        } else {
            gc();
        }
    } else {
        std::cout << "Op ";
        throw c;
    }

}

void Parser::Var() {
    if (!std::isalpha(c)) {
        std::cout << "Var ";
        throw c;
    } else {

        /* < */
        lvalue = true;
        stack.push(c);
        /* > */

        gc();
    }
}

void Parser::Num() {
    if (!std::isdigit(c)) {
        std::cout << "Num ";
        throw c;
    } else {

        /* < */
        lvalue = false;
        stack.push(c - '0');
        /* > */

        gc();
    }
}

int Parser::get_value() const { return value; }

int Parser::get_value_of_var(int operand) {
    if (vars.find(operand) == vars.end()) {
        std::cout << "uninit var ";
        throw c;
    } else {
        return vars[operand];
    }
}


int main() {
    Parser parser;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);
        std::cout << "____________\n";
        std::cout << line << std::endl;
        try
        {
            parser.parse(input);
            std::cout << "YES, value is " << parser.get_value() << std::endl;
            parser.print_vars();
        }
        catch(int c)
        {
            std::cout << std::endl << "NO" << std::endl;
        }
        std::cout << "____________\n";
    }
    return 0;
}
