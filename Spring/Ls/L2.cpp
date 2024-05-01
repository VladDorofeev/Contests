#include <iostream>
#include <exception>
#include <string>
#include <sstream>

class Parser
{
public:
    Parser() = default;
    bool parse(std::istream &);
private:
    char c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void A();
    void B();
};

void Parser::gc() {
    (*in) >> c;
    if ((*in).eof()) {
        c = 0;
    }
}

bool Parser::parse(std::istream &_in) {
    in = &_in;
    try
    {
        gc();
        S();
    }
    catch(char sym)
    {
        std::string temp;
        std::getline((*in), temp);
        return false;
    }

    return (c == 0);
}

void Parser::S() {
    if ((c == 'b') || (c == 'c')) {
        A();
        S();
    } else if ((c == 'd') || (c == 'a')){
        B();
    } else if (c != 0) {
        throw c;
    }
}

void Parser::A() {
    if ((c == 'b') || (c == 'c')) {
        gc();
    } else {
        throw c;
    }
}

void Parser::B() {
    if (c == 'a') {
        gc();
    } else if (c == 'd') {
        gc();
        B();
        if (c == 'f') {
            gc();
        } else {
            throw c;
        }
    }
}

int main() {
    Parser parser;
    std::stringstream input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input << line;
        std::cout << (parser.parse(input) ? "YES" : "NO") << std::endl;
        input.str("");
        input.clear();
    }
    return 0;
}