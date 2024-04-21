#include <iostream>
#include <set>
#include <algorithm>
#include <functional>
using Rule = std::pair<std::string, std::string>;
using Grammar = std::set<Rule>;
using Symbols = std::set<char>;

bool is_terminal(char sym) {
    if (!std::isupper(sym)) {
        return true;
    }
    return false;
}

void make_new_string(Rule const& rule, Symbols& empty_sym, long unsigned int index, std::string str, std::vector<Rule>& comb) {
    if (index == rule.second.size()) {
        if (str != "") {
            comb.push_back({rule.first, str});
        }
        if (str == "" && rule.first == "S") {
            comb.push_back({"T", ""});
            comb.push_back({"T", "S"});
        }
        return;
    }
    if (empty_sym.find(rule.second[index]) != empty_sym.end()) {
        make_new_string(rule, empty_sym, index + 1, str, comb);
    }
    make_new_string(rule, empty_sym, index + 1, str + rule.second[index], comb);
}

void make_new_combinations(Rule const& rule, Symbols& empty_sym, std::vector<Rule>& new_comb) {
    make_new_string(rule, empty_sym, 0, "", new_comb);
}


bool contain_non_good_non_terminal(std::string str, Symbols& non_terminals) {
    if (std::any_of(str.begin(), str.end(), [&non_terminals](char sym){
        if (!isupper(sym)) {
            return false;
        }
        return std::find(non_terminals.begin(), non_terminals.end(), sym) == non_terminals.end() ? true : false;
    })) {
        return true;
    }
    return false;
}

void add_rule(Grammar& g, std::string& s) {
    std::string::size_type delimiter_pos = s.find(':');
    g.insert(Rule(s.substr(0, delimiter_pos), s.substr(delimiter_pos + 1)));
}


void kill_useless_non_terminals(Grammar& g) {
    Symbols good_non_terminals;
    std::for_each(g.begin(), g.end(), [&good_non_terminals](Rule rule){
        if (std::all_of(rule.second.begin(), rule.second.end(), is_terminal)) {
            good_non_terminals.insert(rule.first[0]);
        }
    });
    Symbols::size_type prev_size = good_non_terminals.size();
    while(true) {
        std::for_each(g.begin(), g.end(), [&good_non_terminals](Rule rule){
            if (std::all_of(rule.second.begin(), rule.second.end(), [&good_non_terminals](char& sym){
                if (!std::isupper(sym)) {
                    return true;
                }
                return std::find(good_non_terminals.begin(), good_non_terminals.end(), sym) == good_non_terminals.end() ? false : true;
            })) {
                good_non_terminals.insert(rule.first[0]);
            }
        });
        if (prev_size == good_non_terminals.size()) {
            break;
        }
        prev_size = good_non_terminals.size();
    }
    for(Grammar::iterator it = g.begin(); it != g.end();) {
        if (contain_non_good_non_terminal((*it).first, good_non_terminals)
            || (contain_non_good_non_terminal((*it).second, good_non_terminals))) {
            it = g.erase(it);
        } else {
            ++it;
        }
    }
}


void kill_impossible_sym(Grammar& g) {
    Symbols good_non_terminals;
    if (std::any_of(g.begin(), g.end(), [](Rule const& rule){
        return rule.first == "T";})) {
        good_non_terminals.insert('T');
    } else {
        good_non_terminals.insert('S');
    }
    Symbols::size_type prev_size = good_non_terminals.size();
    while(true) {
        std::for_each(g.begin(), g.end(), [&good_non_terminals](Rule rule){
            if (std::find(good_non_terminals.begin(), good_non_terminals.end(), rule.first[0]) != good_non_terminals.end()) {
                std::for_each(rule.second.begin(), rule.second.end(), [&good_non_terminals](char sym){
                    if (std::isupper(sym)) {
                        good_non_terminals.insert(sym);
                    }
                });
            }
        });
        if (prev_size == good_non_terminals.size()) {
            break;
        }
        prev_size = good_non_terminals.size();
    }
    for(Grammar::iterator it = g.begin(); it != g.end();) {
        if (contain_non_good_non_terminal((*it).first, good_non_terminals)
            || (contain_non_good_non_terminal((*it).second, good_non_terminals))) {
            it = g.erase(it);
        } else {
            ++it;
        }
    }
}


void kill_eps_rules(Grammar& g) {
    Symbols empty_non_terminals;
    for(Grammar::iterator it = g.begin(); it != g.end();) {
        if ((*it).second.empty()) {
            empty_non_terminals.insert(it->first[0]);
            it = g.erase(it);
        } else {
            ++it;
        }
    }
    if (std::find(empty_non_terminals.begin(), empty_non_terminals.end(), 'S') != empty_non_terminals.end()) {
        g.insert({"T", ""});
        g.insert({"T", "S"});
    }
    std::vector<Rule> new_rules;
    for(Grammar::iterator it = g.begin(); it != g.end(); ++it) {
        make_new_combinations(*it, empty_non_terminals, new_rules);
    }
    for(long unsigned int i = 0; i < new_rules.size(); ++i) { 
        g.insert(new_rules[i]);
    }
    for(Grammar::iterator it = g.begin(); it != g.end();) {
        if ((*it).first == (*it).second) {
            it = g.erase(it);
        } else {
            ++it;
        }
    }
}


void make_good_grammar(Grammar& g) {
    kill_useless_non_terminals(g);
    kill_impossible_sym(g);
    kill_eps_rules(g);
    kill_useless_non_terminals(g);
    kill_impossible_sym(g);
}


// S→Ac
// A→SD
// D→aD
// A→a

// S→AB|CD
// A→EF
// G→AD
// C→c
int
main() {
    Grammar grammar;    
    std::string str;
    while (std::getline(std::cin, str)) {
        add_rule(grammar, str);
    }
    make_good_grammar(grammar);
    for(Grammar::iterator it = grammar.begin(); it != grammar.end(); ++it) {
        std::cout << (*it).first << ":" << (*it).second << std::endl;
    }
}
