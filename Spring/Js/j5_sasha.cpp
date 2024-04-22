#include <iostream>
#include <set>
#include <algorithm>


using Rule = std::pair<std::string, std::string>;
using Grammar = std::set<Rule>;
using Symbols = std::set<char>;


std::istream& operator>>(std::istream& in, Grammar& g) {
    std::string str;
    while(std::getline(in, str)) {
        g.insert({str.substr(0, str.find(':')), str.substr(str.find(':') + 1)});
    }
    return in;
}


std::ostream& operator<<(std::ostream& out, Grammar& g) {
    std::for_each(g.begin(), g.end(), [&out](Rule const& rule){
        out << rule.first << ':' << rule.second << std::endl;
    });
    return out;
}



void make_new_string(Rule const& rule, Symbols& empty_sym, long unsigned int index, std::string str, Grammar& comb) {
    if (index == rule.second.size()) {
        if (!str.empty()) {
            comb.insert({rule.first, str});
        }
        return;
    }
    if (empty_sym.find(rule.second[index]) != empty_sym.end()) {
        make_new_string(rule, empty_sym, index + 1, str, comb);
    }
    make_new_string(rule, empty_sym, index + 1, str + rule.second[index], comb);
}


bool contain_non_good_non_terminal(std::string const& str, Symbols& non_terminals) {
    if (std::any_of(str.begin(), str.end(), [&non_terminals](char sym){
        if (!isupper(sym)) {
            return false;
        }
        return std::find(non_terminals.begin(), non_terminals.end(), sym) == non_terminals.end();
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


    //find all left non_terminal like: non_terminal->terminal_str
    std::for_each(g.begin(), g.end(), [&good_non_terminals](Rule rule){
        if (std::all_of(rule.second.begin(), rule.second.end(), [](char sym){
            return !std::isupper(sym);
        })) {
            good_non_terminals.insert(rule.first[0]);
        }
    });


    //find all left non_terminal which right contain only good_non_terminals or terminals
    Symbols::size_type prev_size = good_non_terminals.size();
    do {
        prev_size = good_non_terminals.size();
        std::for_each(g.begin(), g.end(), [&good_non_terminals](Rule rule){
            if (std::all_of(rule.second.begin(), rule.second.end(), [&good_non_terminals](char& sym){
                if (!std::isupper(sym)) {
                    return true;
                }
                return std::find(good_non_terminals.begin(), good_non_terminals.end(), sym) != good_non_terminals.end();
            })) {
                good_non_terminals.insert(rule.first[0]);
            }
        });
    } while(prev_size != good_non_terminals.size());


    //erase all rules which left non_terminal is not in good_non_terminals
    std::erase_if(g, [&good_non_terminals](Rule const& rule){
        return contain_non_good_non_terminal(rule.first, good_non_terminals) 
            || contain_non_good_non_terminal(rule.second, good_non_terminals);
    });
}


void kill_impossible_sym(Grammar& g) {
    Symbols good_non_terminals;


    //add start_sym
    if (std::any_of(g.begin(), g.end(), [](Rule const& rule){
        return rule.first == "T";})) {
        good_non_terminals.insert('T');
    } else {
        good_non_terminals.insert('S');
    }


    //insert all non_terminals like: good_non_terminal->str(non_terminals, terminals)
    Symbols::size_type prev_size = good_non_terminals.size();
    do {
        prev_size = good_non_terminals.size();
        std::for_each(g.begin(), g.end(), [&good_non_terminals](Rule rule){
            if (std::find(good_non_terminals.begin(), good_non_terminals.end(), rule.first[0]) != good_non_terminals.end()) {
                std::for_each(rule.second.begin(), rule.second.end(), [&good_non_terminals](char sym){
                    if (std::isupper(sym)) {
                        good_non_terminals.insert(sym);
                    }
                });
            }
        });
    } while(prev_size != good_non_terminals.size());


    //delete all non_terminal which is not in good_non_terminal
    std::erase_if(g, [&good_non_terminals](Rule const& rule){
        return contain_non_good_non_terminal(rule.first, good_non_terminals) 
            || contain_non_good_non_terminal(rule.second, good_non_terminals);
    });
}


void kill_eps_rules(Grammar& g) {
    Symbols empty_non_terminals;


    //find non_terminals like: non_terminal->empty
    std::erase_if(g, [&empty_non_terminals](Rule const& rule){
        if (rule.second.empty()) {
            empty_non_terminals.insert(rule.first[0]);
            return true;
        }
        return false;
    });


    //find non_terminals like: non_terminal->str(empty_non_terminals)
    Symbols::size_type prev_size = empty_non_terminals.size();
    do {
        prev_size = empty_non_terminals.size();
        std::for_each(g.begin(), g.end(), [&empty_non_terminals](Rule const& rule) {
            if (std::all_of(rule.second.begin(), rule.second.end(), [&empty_non_terminals](char sym){
                return std::find(empty_non_terminals.begin(), empty_non_terminals.end(), sym) != empty_non_terminals.end();
            })){
                empty_non_terminals.insert(rule.first[0]);
            }
        });
    } while(prev_size != empty_non_terminals.size());


    //find all combinations with empty_non_terminals
    Grammar additional_rules;
    std::for_each(g.begin(), g.end(), [&additional_rules, &empty_non_terminals](Rule const& rule){
        make_new_string(rule, empty_non_terminals, 0, "", additional_rules);
    });

    
    //add new rules
    std::for_each(additional_rules.begin(), additional_rules.end(), [&g](Rule const& rule) {
        g.insert(rule);
    });


    //if need add new start symbol
    if (std::find(empty_non_terminals.begin(), empty_non_terminals.end(), 'S') != empty_non_terminals.end()) {
        g.insert({"T", ""});
        g.insert({"T", "S"});
    }


    //delete rules like: X->X
    std::erase_if(g, [&empty_non_terminals](Rule const& rule){
        return rule.first == rule.second;
    });
}


void make_good_grammar(Grammar& g) {
    kill_eps_rules(g);
    kill_useless_non_terminals(g);
    kill_impossible_sym(g);
}


int
main() {
    Grammar grammar;    
    std::cin >> grammar;
    make_good_grammar(grammar);
    std::cout << grammar;
}
