#include <iostream>
#include <exception>
#include <string>
#include <sstream>


/*

S -> <ka = 0; kb = 0;> T < if (cnt_a != 2 * cnt_b) {not in L} >
T -> aT <cnt_a++> | bT <cnt_b++> | eps  

*/

class Parser
{
public:
    Parser() = default;
    bool parse(std::istream &) noexcept;
private:
    int c = 0;
    std::istream *in = nullptr;

    void gc();

    void S();
    void T();
    
    int cnt_a = 0;
    int cnt_b = 0;

};

void Parser::gc() {
    c = in->get();
}

bool Parser::parse(std::istream &_in) noexcept {
    /* < */
    cnt_a = 0;
    cnt_b = 0;
    /* > */

    in = &_in;
    try
    {
        gc();
        S();

        if (c != EOF) {
            throw c;
        }

        /* < */
        if (cnt_a != 2 * cnt_b) {
            throw c;
        }
        /* > */
        
        return true;
    }
    catch(int sym)
    {
        return false;
    }
}

void Parser::S() {
    if (c == 'a' || c == 'b') {
        T();
    } else if (c != EOF){
        throw c;
    }
}
void Parser::T() {
    if (c == 'a') {
        /* < */
        cnt_a++;
        /* > */

        gc();   
        T();
    } else if (c == 'b') {
        /* < */
        cnt_b++;
        /* > */

        gc();
        T();
    }
}

int main() {
    std::cout << "2" << std::endl;
 
    Parser parser;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);
        std::cout << (parser.parse(input) ? "YES" : "NO") << std::endl;
    }
    return 0;
}