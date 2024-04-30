#include <iostream>
#include <exception>


enum Token_kind {OPEN, EXIT, STAR, PLUS, TRAIT, DIGIT, TERMINAL, END_STREAM};


class Token {
public:
    Token() noexcept = default;
    Token(Token_kind, char) noexcept;
    Token(Token const&) noexcept = default;

    Token& operator=(Token const&) noexcept = default;

    Token_kind kind;
    char repr;
};


class TokenStreamIterator {
public:
    TokenStreamIterator() noexcept;
    TokenStreamIterator(std::istream&);
    TokenStreamIterator(TokenStreamIterator const&) noexcept = default;

    bool operator==(TokenStreamIterator const&) const noexcept;
    bool operator!=(TokenStreamIterator const&) const noexcept;
    Token const& operator*() const noexcept;
    Token const* operator->() const noexcept;
    TokenStreamIterator& operator++();
    TokenStreamIterator operator++(int);
private:
    void read_stream();
    std::istream * stream_pt;
    Token token;
};


//Token impl


Token::Token(Token_kind kind_, char repr_) noexcept
    : kind(kind_)
    , repr(repr_)
{
}


//TokenStreamIterator impl


TokenStreamIterator::TokenStreamIterator() noexcept
    : stream_pt(nullptr)
    , token(END_STREAM, '\0')
{
}


TokenStreamIterator::TokenStreamIterator(std::istream& in) 
    : stream_pt(&in)
{
    read_stream();
}


bool TokenStreamIterator::operator==(TokenStreamIterator const& other) const noexcept {
    return stream_pt == other.stream_pt;
}



bool TokenStreamIterator::operator!=(TokenStreamIterator const& other) const noexcept {
    return !(*this == other);
}


Token const& TokenStreamIterator::operator*() const noexcept {
    return token;
}


Token const* TokenStreamIterator::operator->() const noexcept {
    return &token;
}


TokenStreamIterator& TokenStreamIterator::operator++() {
    if (stream_pt) {
        read_stream();
    }
    return *this;
}


TokenStreamIterator TokenStreamIterator::operator++(int) {
    TokenStreamIterator prev(*this);
    if (stream_pt) {
        read_stream();
    }
    return prev;
}


void TokenStreamIterator::read_stream() {
    char sym;
    while(stream_pt->get(sym) && std::isspace(sym));
    if (!(*stream_pt)) {
        token = Token(END_STREAM, '\0');
        stream_pt = nullptr;
        return;
    }
    switch (sym)
    {
    case '(':
        token = Token(OPEN, '(');
        break;
    case ')':
        token = Token(EXIT, ')');
        break;
    case '*':
        token = Token(STAR, '*');
        break;
    case '+':
        token = Token(PLUS, '+');
        break;
    case '|':
        token = Token(TRAIT, '|');
        break;
    case '\\':
        if (!(stream_pt->get(sym))) {
            throw std::exception();
        }
        switch (sym)
        {
        case 'd':
            token = Token(DIGIT, 'd');
            break;
        default:
            std::string str_helper = "()*+|\\ ";
            if (str_helper.find(sym) == std::string::npos) {
                throw std::exception();
            }
            token = Token(TERMINAL, sym);
            break;
        }
        break;
    default:
        token = Token(TERMINAL, sym);
        break;
    }
    
}