#include <iostream>
#include <sstream>
#include <exception>

/*
    //infty space syms
    Tree     = Node
    Node     = List | NonList
    List     = Name()
    NonList  = Name (SubTree)
    Subtree  = Tree | SubTree, Tree
    Name     = chars


    Tree    -> Node ( SubTree )
    SubTree -> eps | Tree {,Tree}
    Node    -> char + some chars and digits

*/


class Parser
{
public:
    Parser() = default;

    void parse(const std::string &s);
private:
    int c;
    
    void gc();
    std::string s;
    std::size_t pos;

    void Tree();
    void SubTree();
    void Node();
};

void Parser::parse(const std::string &_s) {
    s = _s;
    pos = 0;

    gc();
    Tree();

    if (c != EOF) {
        throw std::logic_error("Not EOF");
    }

}
    
void Parser::gc() {
    if (pos == s.length()) {
        c = EOF;
        return;
    }
    c = s.at(pos++);
    while (std::isspace(c)) {
        if (pos == s.length()) {
            c = EOF;
            return;
        }
        c = s.at(pos++);
    }
}

void Parser::Tree() {
    Node();
    if (c == '(') {
        gc();
        SubTree();
        if (c != ')') {
            throw std::logic_error("Not enough )");
        }
        gc();
    } else {
        throw std::logic_error("Not enogh (");
    }
}

void Parser::SubTree() {
    if (std::isalpha(c)) {
        Tree();
        while (c == ',') {
            gc();
            Tree();
        }
    }
}

void Parser::Node() {
    if (std::isalpha(c)) {
        gc();
        while(std::isalnum(c)) {
            gc();
        }
    } else {
        throw std::logic_error("Bad Name");
    }
}

int main() {
    Parser parser;
    
    std::string s;
    while(std::getline(std::cin, s)) {
        try
        {
            parser.parse(s);
            std::cout << "YES" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
            // std::cout << "NO" << std::endl;
        }
    }
    
    return 0;
}