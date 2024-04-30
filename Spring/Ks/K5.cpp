#include <iostream>
#include <exception>
#include <string>

enum TokenType {
    OPENING_PARENTHESIS,
    ENDING_PARENTHESIS,
    ASTERISK,
    PLUS,
    VERTICAL_LINE,
    DIGIT,
    TERM, 
    STREAM_END
};

struct Token
{
    Token() noexcept = default;
    Token(char, TokenType) noexcept;

    char repr;
    TokenType kind;
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
    token(0, STREAM_END)
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
        token = Token(0, STREAM_END);
        in = nullptr;
        return;
    }

    switch (sym)
    {
    case '(':
        token = Token('(', OPENING_PARENTHESIS);
        break;
    case ')':
        token = Token(')', ENDING_PARENTHESIS);
        break;
    case '*':
        token = Token('*', ASTERISK);
        break;
    case '+':
        token = Token('+', PLUS);
        break;
    case '|':
        token = Token('|', VERTICAL_LINE);
        break;
    case '\\':
        if (!(in->get(sym))) {
            throw std::invalid_argument("Stream end, last symbol was \\");
        }

        if (sym == 'd') {
            token = Token('d', DIGIT);
        } else {
            std::string temp = "()*+|\\ ";
            if (temp.find(sym) != std::string::npos) {
                token = Token(sym, TERM);
            } else {
                throw std::invalid_argument("Wrond symbol after \\");
            }
        }
        break;
    default:
        token = Token(sym, TERM);
        break;
    }
}

#ifdef _main
int main() {
    TokenStreamIterator iter(std::cin);
    while (iter->kind != TERM) {
        std::cout << iter->repr << std::endl;
        iter++;
    }
}
#endif