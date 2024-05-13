#include <iostream>
#include <exception>
#include <map>
#include <vector>
#include <cctype>
#include <sstream>
#include <algorithm>


// TREE -> <...>NODE
// NODE -> NAME(LEAFY_OR_NON_LEAFY)
// LEAFY_OR_NON_LEAFY -> SUBTREES | eps
// SUBTREES -> <...>TREE{,<...>TREE}
// NAME -> 'a'<...> | 'b'<...> | ...<...> | 'z'<...> | 'A'<...> | 'B'<...> | ...<...> | 'Z'<...>


using Components = std::map<char, Component *>;
using ComponentsVectors = std::map<int, std::vector<int>>;


class TreeParser {
public:
    Component* parse(std::istream&, std::map<char, Component *> const &);
private:
    void TREE();
    void NODE();
    void LEAFY_OR_NON_LEAFY();
    void SUBTREES();
    void NAME();

    void gc();

    int cur_sym;
    int tree_root;
    int cur_node;
    std::vector<char> names;
    std::istream* stream_pt;
    Components const * components_pt;
    ComponentsVectors components_vectors;
};


//TreeParser impl


Component* TreeParser::parse(std::istream& in, Components const & components) {
    stream_pt = &in;
    components_pt = &components;
    names.clear();
    gc();
    tree_root = cur_sym;

    TREE();
    if (cur_sym != EOF) {
        throw std::runtime_error("End but EOF is impossible");
    }
    if (names.size() != components_pt->size()) {
        throw std::runtime_error("Bad names");
    }
    std::for_each(components_vectors.begin(), components_vectors.end(),
        [&](std::pair<const int, std::vector<int>> const& p){
        std::for_each(p.second.begin(), p.second.end(), [&](int name){
            components_pt->find(p.first)->second->add_subcomponent(components_pt->find(name)->second);
        });
    });
    return components_pt->find(tree_root)->second;
}


void TreeParser::TREE() {
    /*<*/
    cur_node = cur_sym;
    /*>*/
    NODE();
}


void TreeParser::NODE() {
    NAME();
    if (cur_sym == '(') {
        gc();
        LEAFY_OR_NON_LEAFY();
        if (cur_sym != ')') {
            throw std::runtime_error("Bad char: NODE1");
        }
        gc();
    } else {
        throw std::runtime_error("Bad char: NODE2");
    }
}


void TreeParser::LEAFY_OR_NON_LEAFY() {
    if (isalpha(cur_sym)) {
        SUBTREES();
    }
}


void TreeParser::SUBTREES() {
    /*<*/
    int cur_subtree_node = cur_node;
    components_vectors[cur_subtree_node].push_back(cur_sym);
    /*>*/
    TREE();
    while(cur_sym == ',') {
        gc();
        /*<*/
        components_vectors[cur_subtree_node].push_back(cur_sym);
        /*>*/
        TREE();
    }
}


void TreeParser::NAME() {
    if (!isalpha(cur_sym)) {
        throw std::runtime_error("Bad char: NAME");
    }
    /*<*/
    if (components_pt->find(cur_sym) == components_pt->end()) {
        throw std::runtime_error("Bad name");
    }
    names.push_back(cur_sym);
    /*>*/
    gc();
}


void TreeParser::gc() {
    cur_sym = stream_pt->get();
}


Component *make_from_text(std::string const &text, Components const &components) {
    TreeParser tree_parser;
    std::stringstream sin(text);
    return tree_parser.parse(sin, components);
}