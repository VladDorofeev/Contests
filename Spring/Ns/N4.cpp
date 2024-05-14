
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <vector>
#include <algorithm>

/*
    command  ::= seq1
    seq1     ::= seq2 { ('&' | ';') (seq2 | ε) }
        + ε is allowed at the end only
    seq2     ::= pipeline { ('*' | '+') pipeline }
    pipeline ::= redirect { '|' redirect }
    redirect ::= simple { ('<' | '>' ) ID }
    simple   ::= ID { IDargv } | '(' command ')'
    ID       ::= name
    IDargv   ::= name
*/

typedef std::map<char, std::string> Ids;

struct PolizItem
{
    enum Kind {CLOSE, OPEN, REDIRECT, PIPE, BINARY, UNARY, ID, ARGV} kind;
    char repr;
};

typedef std::vector<PolizItem> Poliz;

class Parser
{
public:
    Parser(const Ids&);
    void parse(std::istream &);

    Poliz get_poliz();
private:
    int c = 0;
    std::istream *in = nullptr;

    void gc();

    void Command();
    void Seq1();
    void Seq2();
    void Pipeline();
    void Redirect();
    void Simple();
    void ID();
    void IDargv();
    /* < */
    Ids ids;
    Poliz poliz;
    /* > */

};

Parser::Parser(const Ids &_ids):
    ids(_ids)
{
}

void Parser::gc() {
    c = in->get();
}

void Parser::parse(std::istream &_in) {
    /* < */
    poliz.clear();
    /* > */

    in = &_in;
    gc();
    Command();

    if (c != EOF) {
        throw std::logic_error("Not a EOF");
    }
}

void Parser::Command() {
    Seq1();
}

void Parser::Seq1() {
    Seq2();

    while ((c == '&') || (c == ';')) {
        /* < */
        poliz.push_back({PolizItem::UNARY, static_cast<char>(c)});
        /* > */

        gc();
        if (ids.find(c) != ids.end()) {
            Seq2();
        } else if ((c != EOF) && (c != ')')) {
            throw std::logic_error("Eps is not in the end");
        }
    }
}

void Parser::Seq2() {
    Pipeline();

    while ((c == '*') || (c == '+')) {
        /* < */
        char cmd = c;
        /* > */

        gc();
        Redirect();

        /* < */
        poliz.push_back({PolizItem::BINARY, static_cast<char>(cmd)});
        /* > */
    }
}

void Parser::Pipeline() {
    Redirect(); 

    while (c == '|') {
        gc();
        Redirect();

        /* < */
        poliz.push_back({PolizItem::PIPE, '|'});
        /* > */
    }
}

void Parser::Redirect() {
    Simple();

    while ((c == '>') || (c == '<')) {
        /* < */
        char cmd = c;
        /* > */

        gc();
        ID();

        /* < */
        poliz.push_back({PolizItem::REDIRECT, static_cast<char>(cmd)});
        /* > */
    }
}

void Parser::Simple() {
    if (c != '(') {
        ID();

        while (ids.find(c) != ids.end()) {
            IDargv();
        }
    } else {
        /* < */
        poliz.push_back({PolizItem::OPEN, '('});
        /* > */

        gc();
        Command();
        
        if (c != ')') {
            throw std::logic_error("Not enough ')'");
        }

        /* < */
        poliz.push_back({PolizItem::CLOSE, ')'});
        /* > */

        gc();
    }
}

void Parser::ID() {
    if (ids.find(c) != ids.end()) {
        /* < */
        poliz.push_back({PolizItem::ID, static_cast<char>(c)});
        /* > */

        gc();
    } else {
        throw std::logic_error("Bad ID");
    }
}

void Parser::IDargv() {
    if (ids.find(c) != ids.end()) {
        /* < */
        poliz.push_back({PolizItem::ARGV, static_cast<char>(c)});
        /* > */

        gc();
    } else {
        throw std::logic_error("Bad ID");
    }
}

Poliz Parser::get_poliz() { return poliz; }

void run(Poliz poliz, Ids &ids) {
    std::string cmd;
    std::stack<std::string> stack;

    std::for_each(poliz.begin(), poliz.end(), 
    [&](const PolizItem &item) 
    {
        std::string op1;
        std::string op2;
        std::string res;

        switch (item.kind)
        {
        case PolizItem::ID:
            stack.push(ids[item.repr]);
            break;
        case PolizItem::ARGV:
            op1 = stack.top();
            stack.pop();
            stack.push(op1 + " " + ids[item.repr]);
            break;
        case PolizItem::PIPE:
        case PolizItem::BINARY:
        case PolizItem::REDIRECT:
            op2 = stack.top();
            stack.pop();
            op1 = stack.top();
            stack.pop();
            switch (item.repr)
            {
            case '*':
                res = op1 + " && " + op2;
                break;
            case '+':
                res = op1 + " || " + op2;
                break;
            default:
                res = op1 + " " + std::string(1, item.repr) + " " + op2;
                break;
            }
            stack.push(res);
            break;
        case PolizItem::UNARY:
            op1 = stack.top();
            stack.pop();
            res = op1 + std::string(1, item.repr);
            stack.push(res);
            break;
        case PolizItem::OPEN:
            break;
        case PolizItem::CLOSE:
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


int main() {
    std::string line;
    Ids ids;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        ids[line[0]] = line.substr(1);
    }

    Parser parser(ids);
    while(std::getline(std::cin, line)) {
        try{
            std::stringstream input(line);

            parser.parse(input);

            run(parser.get_poliz(), ids);
        } catch(std::logic_error &err) {
            std::cout << err.what() << std::endl;
        }
    }
    return 0;
}
