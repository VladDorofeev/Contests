#include <iostream>
#include <exception>
#include <cctype>
#include <sstream>

//EXP->CONJ{'|'<...>CONJ<...>}
//CONJ->LOG_OP{'&'<...>LOG_OP<...>}
//LOG_OP->'T'<...>|'F'<...>|DIGIT CMP| (CMP
//CMP->ARI_EXP COND
//COND -> [<...><ARI_EXP|>ARI_EXP|=ARI_EXP<...>]
//ARI_EXP->MUL{+<...>MUL<...>}
//MUL->ARI_OP{*<...>ARI_OP<...>}
//ARI_OP->DIGIT<...>|(EXP)


class Parser {
public:
    void parse(std::istream&);
private:
    void EXP();
    void CONJ();
    void LOG_OP();
    void CMP();
    void COND();
    void ARI_EXP();
    void MUL();
    void ARI_OP();

    void gc();

    int cur_sym;
    std::istream* stream_pt;
    bool is_logic;
};


//Parser impl


void Parser::parse(std::istream& in) {
    stream_pt = &in;
    is_logic = false;
    gc();
    EXP();
    if (cur_sym != EOF) {
        throw std::runtime_error("End but EOF is impossible");
    }
}


void Parser::EXP() {
    CONJ();
    while(cur_sym == '|') {
        /*<*/
        if (!is_logic) {
            throw std::runtime_error("Not logic before |");
        }
        is_logic = false;
        /*>*/
        gc();
        CONJ();
        /*<*/
        if (!is_logic) {
            throw std::runtime_error("Not logic after |");
        }
        /*>*/
    }
}


void Parser::CONJ() {
    LOG_OP();
    while(cur_sym == '&') {
        /*<*/
        if (!is_logic) {
            throw std::runtime_error("Not logic before &");
        }
        is_logic = false;
        /*>*/
        gc();
        LOG_OP();
        /*<*/
        if (!is_logic) {
            throw std::runtime_error("Not logic after &");
        }
        /*>*/
    }
}


void Parser::LOG_OP() {
    if (cur_sym == 'T' || cur_sym == 'F') {
        /*<*/
        is_logic = true;
        /*>*/
        gc();
    } else if (isdigit(cur_sym) || cur_sym == '(') {
        CMP();
    } else {
        throw std::runtime_error("Bad char log1");
    }
}


void Parser::CMP() {
    ARI_EXP();
    COND();
}


void Parser::COND() {
    if (cur_sym == '<' || cur_sym == '>' || cur_sym == '=') {
        /*<*/
        if (is_logic) {
            throw std::runtime_error("Logic before <, >, =");
        }
        /*>*/
        gc();
        ARI_EXP();
        /*<*/
        if (is_logic) {
            throw std::runtime_error("Logic after <, >, =");
        }
        is_logic = true;
        /*>*/
    }
}


void Parser::ARI_EXP() {
    MUL();
    while(cur_sym == '+') {
        /*<*/
        if (is_logic) {
            throw std::runtime_error("Logic before +");
        }
        /*>*/
        gc();
        MUL();
        /*<*/
        if (is_logic) {
            throw std::runtime_error("Logic after +");
        }
        /*>*/
    }
}


void Parser::MUL() {
    ARI_OP();
    while(cur_sym == '*') {
        /*<*/
        if (is_logic) {
            throw std::runtime_error("Logic before *");
        }
        /*>*/
        gc();
        ARI_OP();
        /*<*/
        if (is_logic) {
            throw std::runtime_error("Logic after *");
        }
        /*>*/
    }
}


void Parser::ARI_OP() {
    if (isdigit(cur_sym)) {
        gc();
        /*<*/
        is_logic = false;
        /*>*/
    } else if (cur_sym == '(') {
        gc();
        EXP();
        if (cur_sym != ')') {
            throw std::runtime_error("Bad char ari1");
        }
        gc();
    } else {
        throw std::runtime_error("Bad char ari2");
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
            std::cout << "YES" << std::endl;
        } catch(std::exception& err) {
            std::cout << "NO" << std::endl;
        }
    }
}