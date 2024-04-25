#include <iostream>
#include <set>
#include <string>
#include <algorithm>
#include <unistd.h>

namespace solution
{
typedef std::pair<std::string, std::string> Rule; 
typedef std::set<Rule> Rules;

void grammar_create(Rules &grammar, std::istream &in) 
{
    std::string s;
    while (std::getline(in, s)) {
        std::size_t pos = s.find(":");
        grammar.insert(std::make_pair(s.substr(0,pos), s.substr(pos+1, s.length())));
    }
}
std::string get_str_from_char(char c) {
    char sym_c[2] = {c, 0};
    return std::string(sym_c);
}

std::pair<std::set<std::string>, Rules> get_bad_non_term(Rules &grammar) {
    std::set<std::string> empty_non_term_first;
    std::set<std::string> empty_non_term_second;
    Rules bad_rules;
    
    do {
        empty_non_term_first = empty_non_term_second;
        
        for (Rules::iterator iter = grammar.begin(); iter!= grammar.end(); iter++) {
            if (std::all_of(iter->second.begin(), iter->second.end(),
            [&](char c)
            {
                return (empty_non_term_first.find(get_str_from_char(c)) != empty_non_term_first.end());
            }))
            {
                bad_rules.insert(*iter);
                empty_non_term_second.insert(iter->first);
            }
        }
    } while (empty_non_term_first != empty_non_term_second);
    
    return std::make_pair(empty_non_term_first, bad_rules);
}

void proccess_bad_rule(Rules &grammar, const Rule &r, const std::set<std::string> &bad_non_term, Rules &new_grammar) {
    size_t pos = 0;
    std::for_each(r.second.begin(), r.second.end(), 
    [&](char c)
    {
        if (bad_non_term.contains(get_str_from_char(c))) {
            new_grammar.insert(r);

            std::string temp(r.second);
            temp.erase(temp.find(c, pos), 1);
            pos++;
            if (temp != "") {
                new_grammar.insert(std::make_pair(r.first, temp));
                proccess_bad_rule(grammar, std::make_pair(r.first, temp), bad_non_term, new_grammar);
            }
        }

    });
}

void break_rules(Rules &grammar, std::set<std::string> &bad_non_term, Rules bad_rules) {
    Rules new_grammar;

    std::for_each(grammar.begin(), grammar.end(),
    [&](const Rule &r) 
    {
        if (bad_rules.contains(r)) {
            if (r.first == "S") {
                new_grammar.insert(std::make_pair("T", "S"));
                new_grammar.insert(std::make_pair("T", ""));
            }
            
            if (r.second != "") {
                proccess_bad_rule(grammar, r, bad_non_term, new_grammar);
            } 
            //else rule like (T->eplsilon), don`t add
                
        } else {
            //we need check that right side don`t have bad non term
            if (std::any_of(r.second.begin(), r.second.end(),
            [&](char c){
                return bad_non_term.contains(get_str_from_char(c));
            })
            )
            {
                proccess_bad_rule(grammar, r, bad_non_term, new_grammar);
            } else {
                new_grammar.insert(r);
            }
        }
    }
    );
    
    grammar = new_grammar;
}

void delete_simple_rules(Rules &grammar) {
    Rules new_grammar;

    std::for_each(grammar.begin(), grammar.end(),
    [&](const Rule &r) 
    {
        if (r.first != r.second) {
            new_grammar.insert(r);
        }
    }
    );
    
    grammar = new_grammar;
}

void barren(Rules &grammar) {
    Rules new_grammar;

    std::set<std::string> non_term_first;
    std::set<std::string> non_term_second;

    do {
        non_term_first = non_term_second;
        
        for (Rules::iterator iter = grammar.begin(); iter!= grammar.end(); iter++) {
            if (std::all_of(iter->second.begin(), iter->second.end(),
            [&](char c)
            {
                bool is_term = !std::isupper(c);
                return ((non_term_first.find(get_str_from_char(c)) != non_term_first.end()) || is_term);
            }))
            {
                non_term_second.insert(iter->first);
            }
        }
    } while (non_term_first != non_term_second);

    std::for_each(grammar.begin(), grammar.end(),
    [&](const Rule &r) 
    {
        if (non_term_first.find(r.first) != non_term_first.end()) {
            if (std::all_of(r.second.begin(), r.second.end(),
            [&](char c)
            {
                bool is_term = !std::isupper(c);
                return (non_term_first.find(get_str_from_char(c)) != non_term_first.end()) || is_term;
            }))
            {
                new_grammar.insert(r);
            }
        }
    }
    );

    grammar = new_grammar;
}

void unattainable(Rules &grammar) {
    Rules new_grammar;

    std::set<std::string> non_term_first;
    std::set<std::string> non_term_second;
    
    if (grammar.find({"T", ""}) != grammar.end()) {
        non_term_second.insert("T");
    } else {
        non_term_second.insert("S");
    }

    bool empty_too = false;

    do {
        non_term_first = non_term_second;
        for (Rules::iterator iter = grammar.begin(); iter!= grammar.end(); iter++) {
            if (non_term_first.find(iter->first) != non_term_first.end()) {
                std::for_each(iter->second.begin(), iter->second.end(),
                [&](char c) 
                {
                    non_term_second.insert(get_str_from_char(c));
                }
                );
                if (iter->second.empty()) {
                    empty_too = true;
                }
            }
        }
    } while (non_term_first != non_term_second);

    std::for_each(grammar.begin(), grammar.end(),
    [&](const Rule &r) 
    {
        if (non_term_first.find(r.first) != non_term_first.end()) {
            if (std::all_of(r.second.begin(), r.second.end(),
            [&](char c)
            {
                return (non_term_first.find(get_str_from_char(c)) != non_term_first.end());
            }))
            {
                new_grammar.insert(r);
            }
            if (r.second.empty()) {
                if (empty_too) {
                    new_grammar.insert(r);
                }
            }
        }
    }
    );

    grammar = new_grammar;
}

void normalize(Rules &grammar) {
    //Step 1
    std::pair<std::set<std::string>, Rules> temp = get_bad_non_term(grammar);
    std::set<std::string> bad_non_term = temp.first;
    Rules bad_rules = temp.second;

    //Step 2, 3
    break_rules(grammar, bad_non_term, bad_rules);

    //Remove barren (бесплодные) syms
    barren(grammar);
    
    //Remove unattainable(недостижимые) syms
    unattainable(grammar);

    //Remove rules like X->X
    delete_simple_rules(grammar);
}

void print(Rules &grammar) {
    std::for_each(grammar.begin(), grammar.end(),
    [](const Rule &r) 
    {
        std::cout << r.first << ":" << r.second << std::endl;
    }
    );
}

} 

int main() {
    using namespace solution;
    Rules grammar;
    grammar_create(grammar, std::cin);
    normalize(grammar);
    print(grammar);
}