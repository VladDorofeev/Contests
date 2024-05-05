#include <iostream>
#include <exception>

// S → AD
// D → '|'S | ε
// A → BA | ε
// B → CF
// F → '*'F | '+'F | ε
// C → \d | TERM | '('S')'


class RegexParser {
public:
    RegexParser() noexcept = default;
    void parse(std::istream&);
private:
    void gc();
    void S();
    void A();
    void D();
    void B();
    void F();
    void C();
    std::istream* stream_pt;
    Token token;
};


//RegexParser impl
void RegexParser::parse(std::istream& in) {
    stream_pt = &in;
    gc();
    S();
    if (token.kind != Token::END) {
        throw std::exception();
    }
}


void RegexParser::gc() {
    static TokenStreamIterator it(*stream_pt);
    token = *it;
    ++it;
}


void RegexParser::S() {
    A();
    D();
}


void RegexParser::A() {
    if (token.kind == Token::DIGIT || token.kind == Token::CHAR || token.kind == Token::OPEN)  {
        B();
        A();
    }
}


void RegexParser::D() {
    if (token.kind == Token::BAR) {
        gc();
        S();
    }
}



void RegexParser::B() {
    if (token.kind != Token::DIGIT && token.kind != Token::CHAR && token.kind != Token::OPEN)  {
        throw std::exception();
    } else {
        C();
        F();
    }
}


void RegexParser::F() {
    if (token.kind == Token::STAR || token.kind == Token::PLUS) {
        gc();
        F();
    }
}


void RegexParser::C() {
    if (token.kind != Token::DIGIT && token.kind != Token::CHAR && token.kind != Token::OPEN)  {
        throw std::exception();
    }
    if (token.kind == Token::OPEN) {
        gc();
        S();
        if (token.kind != Token::CLOSE) {
            throw std::exception();
        }
    } 
    gc();
}


int 
main() {
    RegexParser parser;
    try {
        parser.parse(std::cin);
        std::cout << "YES" << std::endl;
    } catch (std::exception&) {
        std::cout << "NO" << std::endl;
    }
}