#include <iostream>
#include <exception>
#include <sstream>

// S -> <stream << 2>aA<stream << a>| bA<stream << b>
// A -> <stream << 2>aA<stream << a> | bA<stream << b> | ε 


class Parser {
public:
    void parse(std::istream&);
    std::string str();
private:
    void S();
    void A();

    void gc();

    std::ostringstream string_out;
    std::istream* stream_pt;
    int cur_sym;
};

//Parser impl
void Parser::parse(std::istream& in) {
    stream_pt = &in;
    string_out.str("");
    gc();
    S();
    if (cur_sym != EOF) {
        throw std::exception();
    }
}


std::string Parser::str() {
    return string_out.str();
}


void Parser::S() {
    if (cur_sym == 'a') {
        /*<*/
        string_out << '2';
        /*>*/
        gc();
        A();
        /*<*/
        string_out << 'a';
        /*>*/
    } else if (cur_sym == 'b') {
        gc();
        A();
        /*<*/
        string_out << 'b';
        /*>*/
    } else {
        throw std::exception();
    }
}


void Parser::A() {
    if (cur_sym == 'a') {
        /*<*/
        string_out << '2';
        /*>*/
        gc();
        A();
        /*<*/
        string_out << 'a';
        /*>*/
    } else if (cur_sym == 'b') {
        gc();
        A();
        /*<*/
        string_out << 'b';
        /*>*/
    }
}


void Parser::gc() {
    cur_sym = stream_pt->get();
}


int 
main() {
    std::string cur_str;
    Parser parser;
    while(std::getline(std::cin, cur_str)) {
        try {
            std::stringstream sin(cur_str);
            parser.parse(sin);
            std::cout << parser.str() << std::endl;
        } catch(std::exception&) {
            std::cout << "NO" << std::endl;
        }
    }
}