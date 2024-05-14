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

using Names = std::map<char, std::string>;
using Poliz = std::vector<Token>;


struct Token
{
    enum Kind {CLOSE, OPEN, REDIRECT, PIPE, CONJ_DISJ, SIGN_SEM, ID, ARGV} kind;
    char repr;
};



class ShellParser {
public:
    ShellParser(Names);

    Poliz& parse(std::istream& );
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


//ShellParser impl


ShellParser::ShellParser(Names _names)
    : names(_names)
{
}


Poliz& ShellParser::parse(std::istream& in) {
    stream_pt = &in;
    poliz.clear();
    gc();
    COMMAND();
    if (cur_sym != EOF) {
        throw std::runtime_error("End but EOF is impossible");
    }
    return poliz;
}


void ShellParser::COMMAND() {
    SEQ1();
}


void ShellParser::SEQ1() {
    SEQ2();
    while(cur_sym == '&' || cur_sym == ';') {
        /*<*/
        poliz.push_back({Token::SIGN_SEM, static_cast<char>(cur_sym)});
        /*>*/
        gc();
        if (names.find(cur_sym) != names.end()) {
            SEQ2();
        } 
        /*<*/
        else if (cur_sym != EOF && cur_sym != ')') {
            throw std::runtime_error("Eps is not in end");
        }
        /*>*/
    }
}


void ShellParser::SEQ2() {
    PIPE();
    while(cur_sym == '*' || cur_sym == '+') {
        /*<*/
        char cur_op = cur_sym;
        /*>*/
        gc();
        PIPE();
        /*<*/
        poliz.push_back({Token::CONJ_DISJ, cur_op});
        /*>*/
    }
}


void ShellParser::PIPE() {
    RED();
    while(cur_sym == '|') {
        gc();
        RED();
        /*<*/
        poliz.push_back({Token::PIPE, '|'});
        /*>*/
    }
}


void ShellParser::RED() {
    SIMPLE();
    while(cur_sym == '<' || cur_sym == '>') {
        /*<*/
        char cur_op = cur_sym;
        /*>*/
        gc();
        if (names.find(cur_sym) == names.end()) {
            throw std::runtime_error("RED: bad ID");
        }
        /*<*/
        poliz.push_back({Token::ID, static_cast<char>(cur_sym)});
        poliz.push_back({Token::REDIRECT, cur_op});
        /*>*/
        gc();
    }
}


void ShellParser::SIMPLE() {
    if (cur_sym == '(') {
        /*<*/
        poliz.push_back({Token::OPEN, static_cast<char>(cur_sym)});
        /*>*/
        gc();
        COMMAND();
        if (cur_sym != ')') {
            throw std::runtime_error("SIMPLE: lost close");
        }
        /*<*/
        poliz.push_back({Token::CLOSE, static_cast<char>(cur_sym)});
        /*>*/
        gc();
    } else if (names.find(cur_sym) != names.end()) {
        /*<*/
        poliz.push_back({Token::ID, static_cast<char>(cur_sym)});
        /*>*/
        gc();
        while(names.find(cur_sym) != names.end()) {
            /*<*/
            poliz.push_back({Token::ARGV, static_cast<char>(cur_sym)});
            /*>*/
            gc();
        }
    } else {
        throw std::runtime_error("SIMPLE: bad sym");
    }
}


void ShellParser::gc() {
    cur_sym = stream_pt->get();
}


//interpret impl


void interpret(Poliz& poliz, Names& names) {
    std::string cmd;
    std::stack<std::string> stack;
    std::for_each(poliz.begin(), poliz.end(), [&](const Token &token) {
        std::string op1;
        std::string op2;
        std::string res;
        switch (token.kind)
        {
        case Token::ID:
            stack.push(names[token.repr]);
            break;
        case Token::ARGV:
            op1 = stack.top();
            stack.pop();
            stack.push(op1 + " " + names[token.repr]);
            break;
        case Token::PIPE:
        case Token::CONJ_DISJ:
        case Token::REDIRECT:
            op2 = stack.top();
            stack.pop();
            op1 = stack.top();
            stack.pop();
            switch (token.repr)
            {
            case '*':
                res = op1 + " && " + op2;
                break;
            case '+':
                res = op1 + " || " + op2;
                break;
            default:
                res = op1 + " " + std::string(1, token.repr) + " " + op2;
                break;
            }
            stack.push(res);
            break;
        case Token::SIGN_SEM:
            op1 = stack.top();
            stack.pop();
            res = op1 + std::string(1, token.repr);
            stack.push(res);
            break;
        case Token::OPEN:
            break;
        case Token::CLOSE:
            op1 = stack.top();
            stack.pop();     
            stack.push("(" + op1 + ")");
            break;
        default:
            break;
        }
    });
    while (!stack.empty()) {
        cmd = stack.top() + cmd;
        stack.pop();
    }
    system(cmd.c_str());
}


int
main() {
    std::string cur_str;
    Names names;
    while(std::getline(std::cin, cur_str)) {
        if (cur_str.empty()) {
            break;
        }
        names[cur_str[0]] = cur_str.substr(1);
    }
    ShellParser shell_parser(names);
    while(std::getline(std::cin, cur_str)) {
        try{
            std::vector<Token> poliz;
            std::istringstream sin(cur_str);
            interpret(shell_parser.parse(sin), names);
        } catch(std::runtime_error& err) {
            std::cout << err.what() << std::endl;
        }
    }
}