#include <iostream>
#include <exception>
#include <string>
#include <vector>

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
    token('e', Token::END)
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
        token = Token('e', Token::END);
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


struct PolizItem
{
    enum Kind { ITER_STAR, ITER_PLUS, OR, CONCAT, DIGIT, CHAR, EMPTY  } kind;
        // EMPTY means empty regex (not EOF)
    char repr; // for kind == CHAR
};

#endif


/*
    Стартовая грамматика:
    Regex   -> Member | Member '|' Regex
    Member  -> eps | Koef | Koef Member
    Koef    -> Element | Koef '*' | Koef '+'
    Element -> \d | term | '(' Regex ')'

    Преобразованная грамматика:

    Regex   -> Member {'|' Member}
    Member  -> Koef { Koef } | eps
    Koef    -> Element { ('+' | '*') }
    Element -> \d | term | '(' Regex ')'

    Добавим действия по построению полиза: 

    Regex     ->    Member {'|' Member <push |>}
    Member    ->    Koef { Koef <push concat> } | eps <push eps>
    Koef      ->    Element { ('+' <push +> | '*' <push *>) }
    Element   ->    \d <push digit> | term <push term> | '(' Regex ')'

*/

typedef std::vector<PolizItem> Poliz;


class RegexParser
{
public:
    RegexParser() = default;

    void parse(std::istream &);

    Poliz::iterator begin();
    Poliz::iterator end();

    Poliz::const_iterator begin() const;
    Poliz::const_iterator end() const;
private:
    Token c;
    TokenStreamIterator iter;

    void gc();

    void Regex();
    void Member();
    void Koef();
    void Element();

    Poliz poliz;
};


void RegexParser::gc() {
    c = *iter;
    ++iter; 
}

void RegexParser::Regex() {
    Member();

    while (c.kind == Token::BAR) {
        gc();
        Member();

        /* < */
        poliz.push_back({PolizItem::OR, '|'});
        /* > */
    }
}

void RegexParser::Member() {
    if ((c.kind == Token::DIGIT) || (c.kind == Token::CHAR) || (c.kind == Token::OPEN)) {
        Koef();

        while ((c.kind == Token::DIGIT) || (c.kind == Token::CHAR) || (c.kind == Token::OPEN)) {
            Koef();

            /* < */
            poliz.push_back({PolizItem::CONCAT, c.repr});
            /* > */
        }
    } else {
        /* < */
        poliz.push_back({PolizItem::EMPTY, 'e'});
        /* > */
    }
}

void RegexParser::Koef() {
    Element();

    while (c.kind == Token::STAR || c.kind == Token::PLUS) {
        /* < */
        if (c.kind == Token::PLUS) {
            poliz.push_back({PolizItem::ITER_PLUS, c.repr});
        } else {
            poliz.push_back({PolizItem::ITER_STAR, c.repr});
        }
        /* > */

        gc();
    }
}

void RegexParser::Element() {
    if (c.kind == Token::DIGIT)  {
        /* < */
        poliz.push_back({PolizItem::DIGIT, c.repr});
        /* > */

        gc();
    } else if (c.kind == Token::CHAR){
        /* < */
        poliz.push_back({PolizItem::CHAR, c.repr});
        /* > */
        
        gc();
    } else if (c.kind == Token::OPEN) {
        gc();
        Regex();
        if (c.kind != Token::CLOSE) {
            throw std::exception();
        }
        gc();
    } else {
        throw std::exception();
    }
}

void RegexParser::parse(std::istream &_in) {
    /* < */
    poliz.clear();
    /* > */

    iter = TokenStreamIterator(_in);
    gc();
    Regex();

    if (c.kind != Token::END) {
        throw std::exception();
    }
}

Poliz::iterator RegexParser::begin() { return poliz.begin(); }
Poliz::iterator RegexParser::end() { return poliz.end(); }

Poliz::const_iterator RegexParser::begin() const { return poliz.begin(); }
Poliz::const_iterator RegexParser::end() const { return poliz.end(); }


#ifdef _main
int main() {
    RegexParser parser;
    try
    {
        parser.parse(std::cin);
        std::cout << "YES" << std::endl;
        for (auto it = parser.begin(); it != parser.end(); ++it) {
            std::cout << it->repr << std::endl;
        } 
    }
    catch(const std::exception& e)
    {
        std::cout << "NO" << std::endl;
    }
}
#endif
