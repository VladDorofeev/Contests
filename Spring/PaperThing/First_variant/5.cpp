#include <iostream>
#include <exception>
#include <string>
#include <sstream>


/*
s is a stream of char

S -> <s.clear()> aT | bT
T -> <s.add('2')> aT <s.add('a')> | bT <s.add('b')> | eps  

tau(w) = s

*/

class Parser
{
public:
    Parser() = default;
    std::string parse(std::istream &);
private:
    int c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void P1();
    void P2();

    std::string s;

};

void Parser::gc() {
    c = in->get();
}

std::string Parser::parse(std::istream &_in) {
    /* < */
    s.clear();
    /* > */

    in = &_in;
    gc();
    S();

    if (c != EOF) {
        throw std::logic_error("BAD");
    }

    return s;
    
}

void Parser::S() {
    P1();
}
void Parser::P1() {
    if (c == 'a') {
        gc();

        if (c != 'b') {
            throw std::logic_error("BAD");
        }
        gc();

        P1();
        
        s.append(1, 'c');
        s.append(1, 'd');
    }  else {
        P2();
    }
}
void Parser::P2() {
    if (c == 'c') {
        gc();

        if (c != 'd') {
            throw std::logic_error("BAD");
        }
        gc();


        s.append(1, 'a');
        s.append(1, 'b');

        P2();
    }
}

int main() {
    Parser parser;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);

        try
        {
            std::string s = parser.parse(input);
            std::cout << s << std::endl;
        }
        catch(std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
