#include <iostream>
#include <exception>
#include <string>
#include <sstream>

#if 0
    S -> bS | aAB
    A -> bcA | ccA | ε
    B -> cbB | ε

    follow(A) = {first(B), follow(S)} = {c}
    first(A) = {b,c}

    follow(A) \cap first(A) = {c} != {}
    => Конфликт first-follow

    Проблема в правиле S->aAB

    Если на взод будет цепочка вида ac...
    то не понятно по какой из альтернатив идти:
    взять A пустой и идти по B (acbB...) 
    или идти по A и потом B. (accA...)
    то есть нужно "видеть" как бы на один символ вперед

    Поэтому попытаемся сделать это всё с помощью грамматики.

    S -> bS | aA 
    A -> bcA | cC | ε
    C -> cA | bB
    B -> cbB | ε

    т.е. просто заменяем правило ccA, разбивая его еще на один нетерминал, 
    чтобы РС "сделал" еще один шаг, тогда он "увидет" еще один символ.
#endif



class Parser
{
public:
    Parser() = default;
    bool parse(std::istream &) noexcept;
private:
    char c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void A();
    void C();
    void B();
};

void Parser::gc() {
    c = in->get();
}

bool Parser::parse(std::istream &_in) noexcept {
    in = &_in;
    try
    {
        gc();
        S();

        if (!(in->eof())) {
            throw c;
        }
        
        return true;
    }
    catch(char sym)
    {
        return false;
    }

}

void Parser::S() {
    if (c == 'b') {
        gc();
        S();
    } else if (c == 'a') {
        gc();
        A();
    } else {
        throw c;
    }
}

void Parser::A() {
    if (c == 'b') {
        gc();
        if (c == 'c') {
            gc();
            A();
        } else {
            throw c;
        }
    } else if (c == 'c') {
        gc();
        C();
    }
}

void Parser::B() {
    if (c == 'c') {
        gc();
        if (c == 'b') {
            gc();
            B();
        } else {
            throw c;
        }
    }
}

void Parser::C() {
    if (c == 'c') {
        gc(); 
        A();
    } else if (c == 'b') {
        gc();
        B();
    } else {
        throw c;
    }
}

int main() {
    Parser parser;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);
        std::cout << (parser.parse(input) ? "YES" : "NO") << std::endl;
    }
    return 0;
}