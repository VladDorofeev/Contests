#include <iostream>
#include <set>
#include <algorithm>
#include <map>

enum {START_STATE};

using Rule = std::pair<char, std::string>;
using Grammar = std::set<Rule>;
using Symbols = std::set<char>;
using States = std::set<Symbols>;
using Map_pairs = std::map<char, Symbols>;
using Automation = std::map<Symbols, Map_pairs>;

Grammar g();


void read_ndfa(Grammar const& g, Automation& a) {
    std::for_each(g.begin(), g.end(), [&a](Rule const& rule){
        if (rule.second.length() == 1) {
            a[{START_STATE}][rule.second[0]].insert(rule.first);
        } else {
            a[{rule.second[0]}][rule.second[1]].insert(rule.first);
        }
    });

}


void make_dfa(Automation& a) {
    Automation dfa_automation;
    
    States states;
    std::for_each(a[{START_STATE}].begin(), a[{START_STATE}].end()
        , [&states, &dfa_automation](std::pair<char const, Symbols>& p){
        dfa_automation[{START_STATE}][p.first] = p.second;
        states.insert(p.second);
    });
    States new_states = states;
    while(!new_states.empty()) {
        std::for_each(new_states.begin(), new_states.end(), [&dfa_automation, &a](Symbols const& syms){
            Map_pairs new_map;
            new_map.clear();
            std::for_each(syms.begin(), syms.end(), [&a, &new_map](char sym){
                std::for_each(a[{sym}].begin(), a[{sym}].end(), [&new_map](std::pair<const char, Symbols>& p){
                    std::set_union(new_map[p.first].begin(), new_map[p.first].end()
                        , p.second.begin(), p.second.end(), std::inserter(new_map[p.first], new_map[p.first].end()));
                });
            });
            dfa_automation[syms] = new_map;
        });
        new_states.clear();
        std::for_each(dfa_automation.begin(), dfa_automation.end()
                , [&states, &new_states](std::pair<const Symbols, Map_pairs>& p){
            std::for_each(p.second.begin(), p.second.end(), [&states, &new_states](std::pair<const char, Symbols>& p1){
                if (std::find(states.begin(), states.end(), p1.second) == states.end()) {
                    states.insert(p1.second);
                    new_states.insert(p1.second);
                }
            });
        });
    }
    a = dfa_automation;
}


bool good_string(Automation& a, std::string const& str) {
    Symbols cur_state {START_STATE};
    for (char sym : str) {
        if (sym == '_') {
            break;
        }
        if (a[cur_state].find(sym) != a[cur_state].end()) {
            cur_state = a[cur_state][sym];
        } else {
            return false;
        }
    }
    if (cur_state.find('S') != cur_state.end() || a[cur_state].find('_') != a[cur_state].end()) {
        return true;
    }
    return false;
}


int
main() {
    Automation automation;
    read_ndfa(g(), automation);
    make_dfa(automation);
    std::string cur_str;
    while(std::getline(std::cin, cur_str)) {
        if (good_string(automation, cur_str)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
} 
