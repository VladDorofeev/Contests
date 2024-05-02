#include <iostream>
#include <exception>
#include <set>
#include <map>
#include <vector>
#include <algorithm>

using Symbols = std::set<char>;
using VectorSyms = std::vector<Symbols>;
using Rules = std::pair<std::string const, std::set<std::string>>;
using Grammar = std::map<std::string, std::set<std::string>>;


std::istream& operator>>(std::istream& in, Grammar& g) {
    std::string str;
    while(std::getline(in, str)) {
        g[str.substr(0, str.find(':'))].insert(str.substr(str.find(':') + 1));
    }
    return in;
}


std::ostream& operator<<(std::ostream& out, Grammar& g) {
    std::for_each(g.begin(), g.end(), [&out](Rules const& rules){
        std::for_each(rules.second.begin(), rules.second.end(), [&out, &rules](std::string str){
            out << rules.first << ':' << str << std::endl;
        });
    });
    return out;
}


bool has_non_terminal_left_rule(Rules const& rules) {
    for(char sym : rules.first) {
        if (std::isupper(sym)) {
            return true;
        }
    }
    return false;
}


bool has_only_one_non_terminal_left(Rules const& rules) {
    if (rules.first.length() == 1 && std::isupper(rules.first[0])) {
        return true;
    }
    return false;
}


bool is_grammar(Grammar const& g) {
    return std::all_of(g.begin(), g.end(), has_non_terminal_left_rule);
}


bool is_ks(Grammar& g) {
    return is_grammar(g) && std::all_of(g.begin(), g.end(), has_only_one_non_terminal_left);
}


void check_first(Symbols& nts, Symbols& syms_, std::string str, Grammar& g, Symbols& nts2) {
    if (str.length() != 0) {
        if (std::isupper(str[0])) {
            if (std::find(nts.begin(), nts.end(), str[0]) == nts.end()) {
                nts.insert(str[0]);
                std::for_each(g[{str[0]}].begin(), g[{str[0]}].end(), [&nts2, &nts, &syms_, &g](std::string const& s){
                    check_first(nts, syms_, s, g, nts2);
                });
            }
            if (std::find(nts2.begin(), nts2.end(), str[0]) != nts2.end()) {
                check_first(nts, syms_, str.substr(1), g, nts2);
            } 
        } else {
            syms_.insert(str[0]);
        }
    }
}


bool crossing_sets(VectorSyms& vec) {
    for (VectorSyms::size_type i = 0; i < vec.size(); ++i) {
        for (VectorSyms::size_type j = i + 1; j < vec.size(); ++j) {
            for (char sym : vec[i]) {
                if (vec[j].count(sym) > 0) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool no_crossing_first(Grammar& g, Symbols& empty_nts) {
    return !std::any_of(g.begin(), g.end(), [&empty_nts, &g](Rules& rules){
        VectorSyms v_syms;
        std::for_each(rules.second.begin(), rules.second.end(), [&empty_nts, &v_syms, &g](std::string const& right){
            Symbols syms;
            Symbols non_terminals;
            check_first(non_terminals, syms, right, g, empty_nts);
            v_syms.push_back(syms);
    
        });
        // std::cout << rules.first << ":";
        // std::for_each(v_syms.begin(), v_syms.end(), [](Symbols const syms){
        //     std::cout << " { ";
        //     std::for_each(syms.begin(), syms.end(), [](char sym){
        //         std::cout << sym << " ";
        //     });
        //     std::cout << "}";
        // }); 
        // std::cout << std::endl;
        return crossing_sets(v_syms);
    });
}

void find_eps_rules(Grammar& g, Symbols& nts) {
    std::for_each(g.begin(), g.end(), [&nts](Rules const& rules){
        if  (std::any_of(rules.second.begin(), rules.second.end(), [](std::string const& str){
                return str.empty();
        })) {
            nts.insert(rules.first[0]);
        }
    });
    Symbols::size_type prev_size = nts.size();
    do {
        prev_size = nts.size();
        std::for_each(g.begin(), g.end(), [&nts](Rules const& rules) {
            if (std::any_of(rules.second.begin(), rules.second.end(), [&nts](std::string const& str){
                return std::all_of(str.begin(), str.end(), [&nts](char sym){
                    return std::find(nts.begin(), nts.end(), sym) != nts.end();
                });
            })) {
                nts.insert(rules.first[0]);
            }
        });
    } while(prev_size != nts.size());
}


bool no_more_one_eps_rules(Grammar& g, Symbols& nts) {
    return !std::any_of(g.begin(), g.end(), [&nts](Rules const& rules){
        int cnt_empty = 0;
        std::for_each(rules.second.begin(), rules.second.end(), [&cnt_empty, &nts](std::string const& str){
            if (std::all_of(str.begin(), str.end(), [&nts](char sym){
                    return std::find(nts.begin(), nts.end(), sym) != nts.end();
            })) {
                ++cnt_empty;
            }
        });
        return cnt_empty > 1;
    });
}


void check_follow(Symbols& nts, Symbols& syms, char nt, std::string const& str, Grammar& g) {
    std::string::size_type cur_pos = str.find(nt);
    if (cur_pos != (str.length() - 1) && cur_pos != std::string::npos) {
        std::string new_str = str.substr(cur_pos + 1);
        for(char sym : new_str) {
            if (std::isupper(sym)) {
                std::for_each(g[{sym}].begin(), g[{sym}].end(), [&nts, &syms, &g](std::string const& right){
                    Symbols non_terminals;
                    check_first(non_terminals, syms, right, g, nts);
                });
                if (std::find(nts.begin(), nts.end(), sym) == nts.end()) {
                    break;
                } else {
                    check_follow(nts, syms, nt, new_str, g);
                }
            } else {
                syms.insert(sym);
            }
        }
    }
}


bool no_crossing_first_with_follow(Grammar& g, Symbols& nts) {
    return !std::any_of(nts.begin(), nts.end(), [&g, &nts](char nt){
        VectorSyms first_and_follow;
        Symbols syms;
        std::for_each(g[{nt}].begin(), g[{nt}].end(), [&nts, &syms, &g](std::string const& right){
            Symbols non_terminals;
            check_first(non_terminals, syms, right, g, nts);
        });
        first_and_follow.push_back(syms);
        syms.clear();
        std::for_each(g.begin(), g.end(), [&nt, &nts, &syms, &g](Rules const rules){
            std::for_each(rules.second.begin(), rules.second.end(), [&nt, &nts, &syms, &g](std::string const& str){
                check_follow(nts, syms, nt, str, g);
            });
        });
        first_and_follow.push_back(syms);
        return crossing_sets(first_and_follow);
    });
}


bool apply_rd(Grammar& g) {
    Symbols empty_non_terminals;
    find_eps_rules(g, empty_non_terminals);
    return is_ks(g) && no_crossing_first(g, empty_non_terminals) && no_more_one_eps_rules(g, empty_non_terminals) &&
        no_crossing_first_with_follow(g, empty_non_terminals);
}


int
main() {
    Grammar grammar;    
    std::cin >> grammar;
    //std::cout << grammar;
    if (apply_rd(grammar)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
}
