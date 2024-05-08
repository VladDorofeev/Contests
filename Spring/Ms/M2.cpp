#include <iostream>
#include <exception>
#include <string>
#include <sstream>


/*

S -> <s.clear()> aT | bT
T -> <s.add('2')> aT <s.add('a')> | bT <s.add('b')> | eps  

*/

class Parser
{
public:
    Parser() = default;
    std::stringstream& parse(std::istream &);
private:
    int c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void T();

    std::stringstream tau;
};

void Parser::gc() {
    c = in->get();
}

std::stringstream& Parser::parse(std::istream &_in) {
    /* < */
    tau.str("");
    /* > */

    in = &_in;
    gc();
    S();

    if (c != EOF) {
        throw c;
    }

    return tau;
    
}

void Parser::S() {
    if (c == 'a' || c == 'b') {
        T();
    } else {
        throw c;
    }
}
void Parser::T() {
    if (c == 'a') {
        /* < */
        tau << '2';
        /* > */

        gc();   
        T();

        /* < */
        tau << 'a';
        /* > */
    } else if (c == 'b') {
        gc();
        T();

        /* < */
        tau << 'b';
        /* > */
    }
}

int main() {
    Parser parser;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);

        try
        {
            std::stringstream &tau = parser.parse(input);
            std::cout << tau.str() << std::endl;
        }
        catch(int c)
        {
            std::cout << "NO" << std::endl;
        }
    }
    return 0;
}