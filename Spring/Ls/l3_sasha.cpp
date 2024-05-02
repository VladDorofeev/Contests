#include <iostream>
#include <sstream>
#include <exception>

//шаг 1
// S → bS | aAB
// A → bcA | ccA | ε // пересечение first(A) и follow(A)
// B → cbB | ε
//сделаем замену


//шаг 2
// S → bS | aС
// С → bcС | ccС | B // пересечение по first()
// B → cbB | ε
//добавим новый нетерминал


//результат
// S → bS | aС
// С → bcС | cD | ε 
// D → cC | bB
// B → cbB | ε



class Parser {
public:
    bool parse(std::istream &);
private:
    void S();
    void C();
    void D();
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
    if (cur_sym == 'b') {
        gc();
        S();
    } else if (cur_sym == 'a') {
        gc();
        C();
    } else {
        throw cur_sym;
    }
}


void Parser::C() {
    if (cur_sym == 'b') {
        gc();
        if (cur_sym == 'c') {
            gc();
            C();
        } else {
            throw cur_sym;
        }
    } else if (cur_sym == 'c') {
        gc();
        D();
    }
}


void Parser::D() {
    if (cur_sym == 'c') {
        gc();
        C();
    } else if (cur_sym == 'b') {
        gc();
        B();
    } else {
        throw cur_sym;
    }
}


void Parser::B() {
    if (cur_sym == 'c') {
        gc();
        if (cur_sym == 'b') {
            gc();
            B();
        } else {
            throw cur_sym;
        }
    }
}


void Parser::gc() {
    cur_sym = stream_pt->get();
}


// S → bS | aС
// С → bcС | cD | ε 
// D → cC | bB
// B → cbB | ε


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