#include <iostream>
#include <exception>
#include <string>

#ifdef _main
struct Token
{
    char repr;
    enum TokenType{OPEN, CLOSE, STAR, PLUS, BAR, DIGIT, CHAR, END} kind;
    Token() noexcept = default;
    Token(char, TokenType) noexcept;

};

class TokenStreamIterator
{
public:
    TokenStreamIterator() noexcept;
    TokenStreamIterator(std::istream &_in);
    TokenStreamIterator(const TokenStreamIterator&) noexcept = default;

    bool operator==(const TokenStreamIterator&) const noexcept;
    bool operator!=(const TokenStreamIterator&) const noexcept;
    
    const Token& operator*() const noexcept;
    const Token* operator->() const noexcept;

    TokenStreamIterator& operator++();
    TokenStreamIterator operator++(int);
private:
    void read();
    std::istream *in; 
    Token token;
};


Token::Token(char sym, TokenType type) noexcept :
    repr(sym),
    kind(type)
{
}


TokenStreamIterator::TokenStreamIterator() noexcept :
    in(nullptr),
    token(0, Token::END)
{
}

TokenStreamIterator::TokenStreamIterator(std::istream &_in) :
    in(&_in)
{
    this->read();
}

bool TokenStreamIterator::operator==(const TokenStreamIterator &other) const noexcept 
{
    return in == other.in; 
}

bool TokenStreamIterator::operator!=(const TokenStreamIterator &other) const noexcept
{
    return !(*this == other);
}

const Token& TokenStreamIterator::operator*() const noexcept 
{
    return token;
}

const Token* TokenStreamIterator::operator->() const noexcept
{
    return &token;
}

TokenStreamIterator& TokenStreamIterator::operator++() 
{
    if (in != nullptr) {
        read();
    }
    return *this;
}

TokenStreamIterator TokenStreamIterator::operator++(int) {
    TokenStreamIterator temp(*this);
    if (in != nullptr) {
        read();
    }
    return temp;

}

void TokenStreamIterator::read() {
    char sym = 0;
    while (in->get(sym) && std::isspace(sym));

    if ((*in).eof()) {
        token = Token(0, Token::END);
        in = nullptr;
        return;
    }

    switch (sym)
    {
    case '(':
        token = Token('(', Token::OPEN);
        break;
    case ')':
        token = Token(')', Token::CLOSE);
        break;
    case '*':
        token = Token('*', Token::STAR);
        break;
    case '+':
        token = Token('+', Token::PLUS);
        break;
    case '|':
        token = Token('|', Token::BAR);
        break;
    case '\\':
        if (!(in->get(sym))) {
            throw std::invalid_argument("Stream end, last symbol was \\");
        }

        if (sym == 'd') {
            token = Token('d', Token::DIGIT);
        } else {
            std::string temp = "()*+|\\ ";
            if (temp.find(sym) != std::string::npos) {
                token = Token(sym, Token::CHAR);
            } else {
                throw std::invalid_argument("Wrond symbol after \\");
            }
        }
        break;
    default:
        token = Token(sym, Token::CHAR);
        break;
    }
}
#endif

/*
    Стартовая грамматика:
    R -> M | M '|' R
    M -> eps | K | KM
    K -> E | K'*' | K '+'
    E -> \d | term | '('R')'


    Заметим, что убрать конфликты по first first можно применив правило:
    T -> A | Aw

    => 

    T -> BC
    C -> eps | w

    Левосторонную рекурсию для K заменим аналогичной правой

    Пользуясь этим и похожими рассуждениями, получим:

    R -> MA
    A -> '|'R | eps
    M -> KM | eps
    K -> ET
    T -> '*'T | '+'T | eps
    E -> \d | term | '('R')'
*/


class RegexParser
{
public:
    RegexParser() = default;

    void parse(std::istream &);
private:
    Token c;
    std::istream *in = nullptr;
    void gc();

    void R();
    void A();
    void M();
    void K();
    void T();
    void E();
};


void RegexParser::gc() {
    static TokenStreamIterator iter(*in);
    c = *iter;
    ++iter; 
}

void RegexParser::R() {
    M(); A();
}

void RegexParser::M() {
    if ((c.kind == Token::DIGIT) || (c.kind == Token::CHAR) || (c.kind == Token::OPEN)) {
        K(); M();
    }
}

void RegexParser::A() {
    if (c.kind == Token::BAR) {
        gc();
        R();
    }
}

void RegexParser::K() {
    E();
    T();
}

void RegexParser::T() {
    if ((c.kind == Token::STAR )|| (c.kind == Token::PLUS)) {
        gc();
        T();
    }
}

void RegexParser::E() {
    if ((c.kind == Token::DIGIT) || (c.kind == Token::CHAR))  {
        gc();
    } else if (c.kind == Token::OPEN) {
        gc();
        R();
        if (c.kind != Token::CLOSE) {
            throw std::exception();
        }
        gc();
    } else {
        throw std::exception();
    }
}

void RegexParser::parse(std::istream &_in) {
    in = &_in;
    gc();
    R();

    if (c.kind != Token::END) {
        throw std::exception();
    }
}

int main() {
    RegexParser parser;
    try
    {
        parser.parse(std::cin);
        std::cout << "YES" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "NO" << std::endl;
    }
}