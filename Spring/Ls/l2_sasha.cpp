#include <iostream>
#include <sstream>
#include <exception>

class Parser {
public:
    bool parse(std::istream &);
private:
    void S();
    void A();
    void B();

    void gc();

    std::istream* stream_pt;
    int cur_sym;
};


//Parser impl
bool Parser::parse(std::istream& stream) {
    stream_pt = &stream;
    try {
        gc();
        S();
        if (cur_sym != EOF) {
            throw cur_sym;
        }
    } catch(int& sym) {
        return false;
    }
    return true;
}


void Parser::S() {
    if (cur_sym == 'b' || cur_sym == 'c') {
        A();
        S();
    } else {
        B();
    }
}
void Parser::A() {
    if (cur_sym == 'b' || cur_sym == 'c') {
        gc();
    } else {
        throw cur_sym;
    }
}
void Parser::B() {
    if (cur_sym == 'a') {
        gc();
    } else if (cur_sym == 'd') {
        gc();
        B();
        if (cur_sym != 'f') {
            throw cur_sym;
        }
        gc();
    }
}

void Parser::gc() {
    cur_sym = stream_pt->get();
}

// S → AS | B
// A → b | c
// B → dBf | a | ε


int 
main() {
    std::string cur_str;
    Parser parser;
    while(std::getline(std::cin, cur_str)) {
        std::stringstream sin(cur_str);
        if (parser.parse(sin)) {
            std::cout << "YES" << std::endl;
        } else {
            std::cout << "NO" << std::endl;
        }
    }
}