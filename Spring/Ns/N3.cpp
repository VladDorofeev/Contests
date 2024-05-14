
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <set>
#include <vector>
#include <algorithm>

/*
    Грамматика:

    <дерево> ::= <вершина>
    <вершина> ::= <листовая вершина> | <нелистовая вершина>
    <листовая вершина> ::= <имя> ()
    <нелистовая вершина> ::= <имя> ( <поддеревья> )
    <поддеревья> ::= <дерево> | <поддеревья> , <дерево>
    <имя> ::= 'a' | 'b' | ... | 'z' | 'A' | 'B' | ... | 'Z'
    
    Приведенная без действий: 

        Tree     ->   Name '(' SubTree ')'
        SubTree  ->   Tree {, Tree} | eps
        Name     ->   char

*/

#ifdef _main
class Component
{
public:
    Component() = default;
    Component(Component const &) = delete;
    void operator=(Component const &) = delete;

    void add_subcomponent(Component *) noexcept {}
private:
};
#endif

typedef std::set<char> Nodes;
typedef std::map<char, Nodes> Connections;

class Parser
{
public:
    Parser() = default;
    void parse(std::istream &);

    Nodes get_nodes();
    Connections get_connects();
    char get_root();
private:
    int c = 0;
    std::istream *in = nullptr;

    void gc();

    void Tree();
    void SubTree();
    void Name();

    Nodes nodes;
    Connections connects;

    char cur_node = -1;
    char tree_root = -1;
};

void Parser::gc() {
    c = in->get();
}

void Parser::parse(std::istream &_in) {
    /* < */
    nodes.clear();
    connects.clear();
    cur_node = -1;
    tree_root = -1;
    /* > */

    in = &_in;
    gc();
    Tree();

    if (c != EOF) {
        throw std::logic_error("Not a EOF");
    }
}

void Parser::Tree() {
    Name();
    if (c == '(') {
        gc();

        SubTree();
        
        if (c != ')') {
            throw std::logic_error("Not enough ')'");
        }
        gc();
    } else {
        throw std::logic_error("Not enough '('");
    }
}

void Parser::SubTree() {
    if (c != ')') {
        /* < */
        char root = cur_node;
        /* > */

        Tree();
        
        /* < */
        if (connects[root].find(cur_node) != connects[root].end()) {
            throw std::logic_error("Double added connection");
        }
        connects[root].insert(cur_node);
        cur_node = root;
        /* > */
        

        while (c == ',') {
            gc();
            Tree();

            /* < */
            if (connects[root].find(cur_node) != connects[root].end()) {
                throw std::logic_error("Double added connection");
            }
            connects[root].insert(cur_node);
            cur_node = root;
            /* > */
        }
    } 
}

void Parser::Name() {
    if (std::isalpha(c)) {
        /* < */
        cur_node = static_cast<char>(c);
        if (nodes.find(cur_node) != nodes.end()) {
            throw std::logic_error("Double used node");
        }
        nodes.insert(cur_node);

        if (tree_root == -1) {
            tree_root = cur_node;
        }
        /* > */

        gc();
    } else {
        throw std::logic_error("Unexceptred symbol");
    }
}


Nodes Parser::get_nodes() { return nodes; }
Connections Parser::get_connects() { return connects; }
char Parser::get_root() { return tree_root; }

Component* make_from_text(std::string const &text, std::map<char, Component *> const &components) {
    Parser parser;
    std::stringstream input(text);

    parser.parse(input);

    Nodes nodes = parser.get_nodes();
    Connections connects = parser.get_connects();

    if (nodes.size() != components.size()) {
        throw std::runtime_error("Bad components");
    }

    //Just check components
    std::for_each(nodes.begin(), nodes.end(),
    [&](char node)
    {
        if (components.find(node) == components.end()) {
            throw std::runtime_error("No node in components");
        }
    });

    //Work with components
    std::for_each(nodes.begin(), nodes.end(),
    [&](char node)
    {
        std::for_each(connects[node].begin(), connects[node].end(),
        [&](char subnode)
        {
            components.at(node)->add_subcomponent(components.at(subnode));
        });
    });

    return components.at(parser.get_root());
}


#ifdef _main

// int main() {
//     Parser parser;
    
//     std::string line;
//     while (std::getline(std::cin, line)) {
//         std::stringstream input(line);
//         try
//         {
//             parser.parse(input);
//             std::cout << "YES" << std::endl;
//             std::cout << "root = " << parser.get_root() << std::endl;
//             Nodes nodes = parser.get_nodes();
//             std::for_each(nodes.begin(), nodes.end(),
//             [](char c)
//             {
//                 std::cout << c <<  ", ";
//             });
//             std::cout << std::endl;

//             Connections connects = parser.get_connects();

//             std::for_each(connects.begin(), connects.end(),
//             [](auto pair)
//             {
//                 std::cout << pair.first << " : ";
//                 std::for_each(pair.second.begin(), pair.second.end(),
//                 [](char c)
//                 {
//                     std::cout << c <<  ", ";
//                 });
//                 std::cout << std::endl;
//             });
//             std::cout << std::endl;
//         }
//         catch(const std::exception &err)
//         {
//             std::cout << "NO, err = " << err.what() << std::endl;
//             // std::cout << "NO" << std::endl;
//         }
//     }
//     return 0;
// }

using Components = std::map<char, Component *>;
using ComponentsVectors = std::map<int, std::vector<int>>;
void test_parse_tree() {
    Component a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;

    Components components = {
        {'a', &a},
        {'b', &b},
        {'c', &c},
        {'d', &d},
        {'e', &e},
        {'f', &f},
        {'g', &g},
        {'h', &h},
        {'i', &i},
        {'j', &j},
        {'k', &k},
        {'l', &l},
        {'m', &m},
        {'n', &n},
        {'o', &o},
        {'p', &p},
        {'q', &q},
        {'r', &r},
        {'s', &s},
        {'t', &t},
        {'u', &u},
        {'v', &v},
        {'w', &w},
        {'x', &x},
        {'y', &y},
        {'z', &z}
    };

    std::string text = "a(b(c(d(),e(),f(g(h(),i())),j(k(),l())),m(),n(o(p(),q()),r(),s(t(),u()))),v(w(x(),y()),z()))";

    try {
        make_from_text(text, components);
        std::cout << "Complex tree parsed successfully!" << std::endl;
    } catch(std::runtime_error& err) {
        std::cout << "Failed to parse complex tree: " << err.what() << std::endl;
    }
}

int main() {
    test_parse_tree();
    return 0;
}
#endif
