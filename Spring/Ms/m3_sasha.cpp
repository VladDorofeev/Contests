#include <iostream>
#include <exception>
#include <cctype>
#include <map>
#include <stack>
#include <sstream>

//EXP->ASS{,ASS}
//ASS -><...>OP_OR_SUB ASS_OR_EPS
//ASS_OR_EPS -> =<...>ASS<...> | eps<...>
//OP_OR_SUB->OP{ -<...>OP<...>}
//OP->VAR|NUM|(EXP)<...>
//VAR->ALPHA<...>
//NUM->DIGIT<...>


class Parser {
public:
    void parse(std::istream&);
    int res();
private:
    void EXP();
    void ASS();
    void ASS_OR_EPS();
    void OP_OR_SUB();
    void OP();
    void VAR();
    void NUM();

    void gc();

    int cur_sym;
    std::istream* stream_pt;
    std::map<int, int> tid;
    std::stack<int> ops;
    int cur_op;
    bool is_var;
};


//Parser impl


void Parser::parse(std::istream& in) {
    stream_pt = &in;
    while(!ops.empty()) {
        ops.pop();
    }
    tid.clear();
    EXP();
    if (cur_sym != EOF) {
        throw std::runtime_error("End but EOF is impossible");
    }
    
}


void Parser::EXP() {
    gc();
    ASS();
    while (cur_sym == ',')
    {
        gc();
        ASS();
    }
    
}


void Parser::ASS() {
    /*<*/
    is_var = false;
    /*>*/
    OP_OR_SUB();
    ASS_OR_EPS();
}


void Parser::ASS_OR_EPS() {
    if (cur_sym == '=') {
        /*<*/
        if (!is_var) {
            throw std::runtime_error("Not var before assign");
        }
        ops.push(cur_op);
        /*>*/
        gc();
        ASS();
        /*<*/
        int cur_val = ops.top();
        ops.pop();
        int cur_var = ops.top();
        ops.pop();
        ops.push(cur_val);
        tid[cur_var] = cur_val;
        /*>*/
    } else {
        /*>*/
        if (is_var && tid.find(cur_op) == tid.end()) {
            throw std::runtime_error("Var is not init");
        } else if (is_var) {
            ops.push(tid[cur_op]);
        } else {
            ops.push(cur_op);
        }
        /*<*/
    }
}


void Parser::OP_OR_SUB() {
    OP();
    if (cur_sym == '-') {
        /*<*/
        int sub_res;
        if (is_var && tid.find(cur_op) == tid.end()) {
            throw std::runtime_error("Var is not init");
        } else if (is_var) {
            sub_res = tid[cur_op];
        } else {
            sub_res = cur_op;
        }
        is_var = false;
        /*>*/
        while(cur_sym == '-') {
            gc();
            OP();
            /*<*/
            if (is_var && tid.find(cur_op) == tid.end()) {
                throw std::runtime_error("Var is not init");
            } else if (is_var) {
                sub_res -= tid[cur_op];
            } else {
                sub_res -= cur_op;
            }
            is_var = false;
            /*>*/
        }
        /*<*/
        cur_op = sub_res;
        /*>*/
    }
}


void Parser::OP() {
    if (isalpha(cur_sym)) {
        VAR();
    } else if (isdigit(cur_sym)) {
        NUM();
    } else if (cur_sym == '(') {
        EXP();
        if (cur_sym != ')') {
            throw std::runtime_error("Not found close )");
        }
        /*<*/
        cur_op = ops.top();
        ops.pop();
        /*>*/
        gc();
    } else {
        throw std::runtime_error("Bad char");
    }
}


void Parser::VAR() {
    if (!isalpha(cur_sym)) {
        throw std::runtime_error("Bad char");
    }
    /*<*/
    is_var = true;
    cur_op = cur_sym;
    /*>*/
    gc();
}


void Parser::NUM() {
    if (!isdigit(cur_sym)) {
        throw std::runtime_error("Bad char");
    }
    /*<*/
    cur_op = cur_sym - '0';
    /*>*/
    gc();
}


void Parser::gc() {
    cur_sym = stream_pt->get();
}


int Parser::res() {
    return ops.top();
}


int 
main() {
    std::string cur_str;
    Parser parser;
    while(std::getline(std::cin, cur_str)) {
        try {
            std::stringstream sin(cur_str);
            parser.parse(sin);
            std::cout << parser.res() << std::endl;
        } catch(std::exception& err) {
            std::cout << "NO" << std::endl;
        }
    }
}