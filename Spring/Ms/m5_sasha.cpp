#include <iostream>
#include <vector>
#include <exception>


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
