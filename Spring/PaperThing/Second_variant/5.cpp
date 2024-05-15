#include <iostream>
#include <exception>
#include <string>
#include <sstream>


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
    void A();
    void B();
    void C();

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
    A();
}

void Parser::A() {
    if (c == 'a') {
        gc();
        A();
        s.append(1, 'c');
    } else {
        B();
    }
}

void Parser::B() {
    if (c == 'b') {
        s.append(1, 'a');
        
        gc();
        B();
    } else {
        C();
    }
}

void Parser::C() {
    if (c == 'c') {
        gc();
        C();

        s.append(1, 'b');
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
