#include <iostream>
#include <vector>
#include <exception>
#include <sstream>


class Token {
public:
    enum Kind { OPEN, CLOSE, STAR, PLUS, BAR, DIGIT, CHAR, END};
    Token() noexcept = default;
    Token(Token::Kind, char) noexcept;
    Token(Token const&) noexcept = default;

    Token& operator=(Token const&) noexcept = default;

    Kind kind;
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


Token::Token(Token::Kind kind_, char repr_) noexcept
    : kind(kind_)
    , repr(repr_)
{
}


//TokenStreamIterator impl


TokenStreamIterator::TokenStreamIterator() noexcept
    : stream_pt(nullptr)
    , token(Token::END, '\0')
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
        token = Token(Token::END, '\0');
        stream_pt = nullptr;
        return;
    }
    switch (sym)
    {
    case '(':
        token = Token(Token::OPEN, '(');
        break;
    case ')':
        token = Token(Token::CLOSE, ')');
        break;
    case '*':
        token = Token(Token::STAR, '*');
        break;
    case '+':
        token = Token(Token::PLUS, '+');
        break;
    case '|':
        token = Token(Token::BAR, '|');
        break;
    case '\\':
        if (!(stream_pt->get(sym))) {
            throw std::exception();
        }
        switch (sym)
        {
        case 'd':
            token = Token(Token::DIGIT, 'd');
            break;
        default:
            std::string str_helper = "()*+|\\ ";
            if (str_helper.find(sym) == std::string::npos) {
                throw std::exception();
            }
            token = Token(Token::CHAR, sym);
            break;
        }
        break;
    default:
        token = Token(Token::CHAR, sym);
        break;
    }
    
}

struct PolizItem
{
enum Kind { ITER_STAR, ITER_PLUS, OR, CONCAT, DIGIT, CHAR, EMPTY  } kind;
    // EMPTY means empty regex (not EOF)
char repr; // for kind == CHAR
};
   
// REGEX -> MEMBER{'|' MEMBER<...>}
// MEMBER -> eps<...> | COEFF{COEFF<...>}
// COEFF -> ELEM{'*'<...>|'+'<...>}
// ELEM -> DIGIT<...> | CHAR<...> | '(' REGEX ')'


using PolizItems = std::vector<PolizItem>;

    
class RegexParser {
public:
    RegexParser() noexcept = default;

    void parse(std::istream&);
    PolizItems::iterator begin();
    PolizItems::iterator end();
    PolizItems::const_iterator begin() const;
    PolizItems::const_iterator end() const;
private:
    void gc();
    
    void REGEX();
    void MEMBER();
    void COEFF();
    void ELEM();

    TokenStreamIterator it;
    PolizItems poliz_items;
    Token token;
};


//RegexParser impl
void RegexParser::parse(std::istream& in) {
    poliz_items.clear();
    it = TokenStreamIterator(in);
    gc();
    REGEX();
    if (token.kind != Token::END) {
        throw std::exception();
    }
}


PolizItems::iterator RegexParser::begin() {
    return poliz_items.begin();
}


PolizItems::iterator RegexParser::end() {
    return poliz_items.end();
}


PolizItems::const_iterator RegexParser::begin() const {
    return poliz_items.begin();
}


PolizItems::const_iterator RegexParser::end() const {
    return poliz_items.end();
}


void RegexParser::gc() {
    token = *it;
    ++it;
}


void RegexParser::REGEX() {
    MEMBER();
    while (token.kind == Token::BAR) {
        gc();
        MEMBER();
        /*<*/
        poliz_items.push_back({PolizItem::OR, '\0'});
        /*>*/
    }
    
}


void RegexParser::MEMBER() {
    if (token.kind == Token::DIGIT || token.kind == Token::CHAR || token.kind == Token::OPEN) {
        COEFF();
        while(token.kind == Token::DIGIT || token.kind == Token::CHAR || token.kind == Token::OPEN) {
            COEFF();
            /*<*/
            poliz_items.push_back({PolizItem::CONCAT, '\0'});
            /*>*/
        }
    } else {
        /*<*/
        poliz_items.push_back({PolizItem::EMPTY, '\0'});
        /*>*/
    }
}


void RegexParser::COEFF() {
    ELEM();
    while(token.kind == Token::STAR || token.kind == Token::PLUS) {
        /*<*/
        if (token.kind == Token::PLUS) {
            poliz_items.push_back({PolizItem::ITER_PLUS, '\0'});
        } else if (token.kind == Token::STAR) {
            poliz_items.push_back({PolizItem::ITER_STAR, '\0'});
        }
        /*>*/
        gc();
    }
}


void RegexParser::ELEM() {
    if (token.kind == Token::DIGIT) {
        /*<*/
        poliz_items.push_back({PolizItem::DIGIT, '\0'});
        /*>*/
        gc();
    } else if (token.kind == Token::CHAR) {
        /*<*/
        poliz_items.push_back({PolizItem::CHAR, token.repr});
        /*>*/
        gc();
    } else if (token.kind == Token::OPEN) {
        gc();
        REGEX();
        if (token.kind != Token::CLOSE) {
            throw std::exception();
        }
        gc();
    } else {
        throw std::exception();
    }
}


class RegexMatcher {
public:
    RegexMatcher(std::string const&);

    bool match(std::string const&);
private:
    RegexParser parser;
};


//RegexMatcher impl


RegexMatcher::RegexMatcher(std::string const& re) {
    std::istringstream sin(re);
    parser.parse(sin);
}


bool RegexMatcher::match(std::string const& str) {
 std::vector<std::string::const_iterator> stack;
    stack.push_back(str.begin());

    for (auto it = parser.begin(); it != parser.end(); ++it) {
        switch (it->kind) {
            case PolizItem::CHAR: {
                if (stack.empty() || *stack.back() != it->repr) {
                    return false;
                }
                ++stack.back();
                break;
            }
            case PolizItem::DIGIT: {
                if (stack.empty() || !isdigit(*stack.back())) {
                    return false;
                }
                ++stack.back();
                break;
            }
            case PolizItem::EMPTY: {
                break;
            }
            case PolizItem::CONCAT: {
                break;
            }
            case PolizItem::ITER_PLUS: {
                if (stack.empty() || *stack.back() == '\0') {
                    return false;
                }
                while (*stack.back() != '\0') {
                    auto backup = stack.back();
                    for (auto sub_it = it; sub_it != parser.end(); ++sub_it) {
                        if (sub_it->kind == PolizItem::ITER_STAR) {
                            break;
                        }
                        if (!match(str)) {
                            stack.back() = backup;
                            break;
                        }
                    }
                    ++stack.back();
                }
                break;
            }
            case PolizItem::ITER_STAR: {
                auto backup = stack.back();
                while (*stack.back() != '\0') {
                    if (!match(str)) {
                        stack.back() = backup;
                        break;
                    }
                }
                break;
            }
            case PolizItem::OR: {
                auto backup = stack.back();
                if (!match(str)) {
                    stack.back() = backup;
                }
                break;
            }
        }
    }

    return stack.back() == str.end();
}


int 
main() {
    std::string cur_str;
    if (std::getline(std::cin, cur_str)) {
        try {
            RegexMatcher matcher(cur_str);
            while(std::getline(std::cin, cur_str)) {
                if (matcher.match(cur_str)) {
                    std::cout << "YES" << std::endl;
                } else {
                std::cout << "NO" << std::endl;
                }
            }
        } catch(...) {
            std::cout << "INCORRECT REGEX" << std::endl;
            return 0;
        }
    } else {
        return 0;
    }
}