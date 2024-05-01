#include <iostream>
#include <exception>
#include <string>
#include <sstream>

class Parser
{
public:
    Parser() = default;
    bool parse(std::istream &) noexcept;
private:
    char c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void A();
    void B();
};

void Parser::gc() {
    c = in->get();
}

bool Parser::parse(std::istream &_in) noexcept {
    in = &_in;
    try
    {
        gc();
        S();

        if (!(in->eof())) {
            throw c;
        }
        
        return true;
    }
    catch(char sym)
    {
        return false;
    }

}

void Parser::S() {
    if ((c == 'b') || (c == 'c')) {
        A();
        S();
    } else {
        B();
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
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);
        std::cout << (parser.parse(input) ? "YES" : "NO") << std::endl;
    }
    return 0;
}