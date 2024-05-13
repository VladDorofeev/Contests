#include <iostream>
#include <exception>
#include <map>
#include <stack>
#include <vector>
#include <cctype>
#include <sstream>
#include <algorithm>


//COMMAND -> SEQ1
//SEQ1 -> SEQ2{('&' | ';') (SEQ2 | eps)}
                            //+ ε is allowed at the end only
//SEQ2 -> PIPE{('*' | '+')PIPE}
//PIPE -> RED{'|' RED}
//RED -> SIMPLE{('<' | '>') ID}
//SIMPLE -> ID{ID}|'('COMMAND')'

struct Token;

using Names = std::map<int, std::string>;
using Poliz = std::vector<Token>;


struct Token
{
    enum Kind {CLOSE, OPEN, REDIRECT, PIPE, CONJ_DISJ, SIGN_SEM, ID, ARGV} kind;
    int repr;
};



class ShellParser {
public:
    ShellParser(Names);

    Poliz parse(std::istream& );
private:
    void COMMAND();
    void SEQ1();
    void SEQ2();
    void PIPE();
    void RED();
    void SIMPLE();

    void gc();
    Names names;
    std::istream* stream_pt;
    int cur_sym;
    std::vector<Token> poliz;
};


class Interpreter {
public:
    Interpreter(Names names_);

    void interpret(Poliz);
private:
    Poliz poliz;
    Names names;
};


//ShellParser impl


ShellParser::ShellParser(Names _names)
    : names(_names)
{
}


Poliz ShellParser::parse(std::istream& in) {
    stream_pt = &in;
    poliz.clear();
    gc();
    COMMAND();
    if (cur_sym != EOF) {
        throw std::runtime_error("End but EOF is impossible");
    }
    std::cout << "Poliz ";
    std::for_each(poliz.begin(), poliz.end(), [](Token const& str){
        std::cout << char(str.repr) << ' ';
    });
    std::cout << std::endl;
    return poliz;
}


void ShellParser::COMMAND() {
    SEQ1();
}


void ShellParser::SEQ1() {
    SEQ2();
    while(cur_sym == '&' || cur_sym == ';') {
        poliz.push_back({Token::SIGN_SEM, cur_sym});
        gc();
        if (names.find(cur_sym) != names.end()) {
            SEQ2();
        } else if (cur_sym != EOF && cur_sym != ')') {
            throw std::runtime_error("Eps is not in end");
        }
    }
}


void ShellParser::SEQ2() {
    PIPE();
    while(cur_sym == '*' || cur_sym == '+') {
        int cur_op = cur_sym;
        gc();
        PIPE();
        poliz.push_back({Token::CONJ_DISJ, cur_op});
    }
}


void ShellParser::PIPE() {
    RED();
    while(cur_sym == '|') {
        gc();
        RED();
        poliz.push_back({Token::PIPE, '|'});
    }
}


void ShellParser::RED() {
    SIMPLE();
    while(cur_sym == '<' || cur_sym == '>') {
        int cur_op = cur_sym;
        gc();
        if (names.find(cur_sym) == names.end()) {
            throw std::runtime_error("RED: bad ID");
        }
        poliz.push_back({Token::ID, cur_sym});
        poliz.push_back({Token::REDIRECT, cur_op});
        gc();
    }
}


void ShellParser::SIMPLE() {
    if (cur_sym == '(') {
        poliz.push_back({Token::OPEN, cur_sym});
        gc();
        COMMAND();
        if (cur_sym != ')') {
            throw std::runtime_error("SIMPLE: lost close");
        }
        poliz.push_back({Token::CLOSE, cur_sym});
        gc();
    } else if (names.find(cur_sym) != names.end()) {
        poliz.push_back({Token::ID, cur_sym});
        gc();
        while(names.find(cur_sym) != names.end()) {
            poliz.push_back({Token::ARGV, cur_sym});
            gc();
        }
    } else {
        throw std::runtime_error("SIMPLE: bad sym");
    }
}


void ShellParser::gc() {
    cur_sym = stream_pt->get();
}


//Interpreter impl


Interpreter::Interpreter(Names names_) 
    : names(names_)
{
}


void Interpreter::interpret(Poliz poliz_) {
    poliz = poliz_;
    // std::string command;
    // std::stack<Token> args;
    // Poliz::size_type index = 0;
    // Poliz::size_type size = poliz.size();
    // Token poliz_lex;
    // while(index < size) {

    // }
    std::cout << "----------------" << std::endl;
    std::string cmd;
    std::stack<std::string> stack;
    char temp[2];
    std::for_each(poliz.begin(), poliz.end(),
    [&](const Token &token)
    {
        std::cout << static_cast<char>(token.repr) << std::endl;
        if (token.kind == Token::ID) {
            std::cout << "pushed " << names[token.repr] << std::endl; 
            stack.push(names[token.repr]);
        } else if ((token.kind != Token::SIGN_SEM) && (token.kind != Token::ARGV)){
            std::string op2 = stack.top();
            stack.pop();
            std::string op1 = stack.top();
            stack.pop();

            temp[0] = token.repr;
            std::string res = op1 + std::string(temp) + op2;
            std::cout << "parse " << temp[0] << " str = " << res << std::endl;
            stack.push(res);

        } else if (token.kind == Token::SIGN_SEM) {
            std::string op1 = stack.top();
            stack.pop();

            temp[0] = token.repr;
            std::string res = op1 + std::string(temp);
            std::cout << "parse " << temp[0] << " str = " << res << std::endl;
            stack.push(res);
        } else //here argv 
        {
            std::string op1 = stack.top();
            stack.pop();
            stack.push(op1 + " " + names[token.repr]);
        }

    });
    while (!stack.empty()) {
        cmd = stack.top() + cmd;
        stack.pop();
    }
    std::cout << cmd << std::endl;
    if (!stack.empty()) {
        std::cout << "HEUTA" << std::endl;
    }

    // a<b & a>c+a>b<c;

    std::cout << "----------------" << std::endl;

}


int
main() {
    std::string cur_str;
    Names names;
    std::getline(std::cin, cur_str);
    while(!cur_str.empty()) {
        if (cur_str.length() <= 1) {
            throw std::runtime_error("Input: bad str");
        }
        if (names.find(cur_str[0]) != names.end()) {
            throw std::runtime_error("Input: bad str 2");
        }
        names[cur_str[0]] = cur_str.substr(1);
        std::getline(std::cin, cur_str);
    }
    std::for_each(names.begin(), names.end(), [&](std::pair<const int, std::string> const& p){
        std::cout << "ID: " << p.first << " Name: " << p.second << std::endl;
    });
    ShellParser shell_parser(names);
    Interpreter interpreter(names);
    while(std::getline(std::cin, cur_str)) {
        try{
            std::vector<Token> poliz;
            std::istringstream sin(cur_str);
            poliz = shell_parser.parse(sin);
            interpreter.interpret(poliz);
        } catch(std::runtime_error& err) {
            std::cout << err.what() << std::endl;
        }
    }
}