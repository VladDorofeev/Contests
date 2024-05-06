#include <iostream>
#include <exception>
#include <sstream>

// S → <ka = 0; kb = 0;> A <if (ka != 2*kb){throw std::exception;}>
// A → aA <ka++> | bA <kb++> | ε  

class Parser {
public:
    void parse(std::istream&);
private:
    void S();
    void A();

    void gc();

    std::istream* stream_pt;
    int cur_sym;
    int ka;
    int kb;
};

//Parser impl
void Parser::parse(std::istream& in) {
    stream_pt = &in;
    gc();
    S();
}


void Parser::S() {
    /*<*/
    ka = 0;
    kb = 0;
    /*>*/
    A();
}


void Parser::A() {
    if (cur_sym == 'a') {
        /*<*/
        ++ka;
        /*>*/
    } else if (cur_sym == 'b') {
        /*<*/
        ++kb;
        /*>*/
    } else if (cur_sym == EOF) {
        /*<*/
        if (ka != 2*kb) {
            throw std::exception();
        }
        /*>*/
    } else {
        throw std::exception();
    }
    if (cur_sym == 'a' || cur_sym == 'b') {
        gc();
        A();
    }
}


void Parser::gc() {
    cur_sym = stream_pt->get();
}

int 
main() {
    std::cout << 2 << std::endl;
    std::string cur_str;
    Parser parser;
    while(std::getline(std::cin, cur_str)) {
        try {
            std::stringstream sin(cur_str);
            parser.parse(sin);
            std::cout << "YES" << std::endl;
        } catch(std::exception&) {
            std::cout << "NO" << std::endl;
        }
    }
}
