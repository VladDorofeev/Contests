#include <iostream>
#include <set>
#include <string>
#include <algorithm>
#include <unistd.h>

namespace solution
{
typedef std::pair<std::string, std::string> Rule_type; 
typedef std::set<Rule_type> Grammar_type;

class Grammar
{
public:
    Grammar(std::istream &);
    Grammar(const Grammar &) = delete;
    Grammar& operator=(const Grammar &) = delete;
    ~Grammar() = default;


    void cast();
    void print() const;
private:
    //Useful methods
    std::string get_str_from_char(char c);

    //Step 1
    std::pair<std::set<std::string>, Grammar_type> get_bad_non_term();
    
    //Step 2 (and 3 at the same moment)
    void break_rules(std::set<std::string> &bad_non_term, Grammar_type bad_rules);
    void proccess_bad_rule(const Rule_type &, const std::set<std::string> &, Grammar_type &);

    //Remove X->X
    void delete_simple_rules();

    //Remove barren syms
    void barren();
    
    //Remove unattainable syms
    void unattainable();

    //Fields
    Grammar_type items;
};

Grammar::Grammar(std::istream &in) 
{
    std::string s;
    while (std::getline(in, s)) {
        std::size_t pos = s.find(":");
        items.insert(std::make_pair(s.substr(0,pos), s.substr(pos+1, s.length())));
    }
}
std::string Grammar::get_str_from_char(char c) {
    char sym_c[2] = {c, 0};
    return std::string(sym_c);
}

std::pair<std::set<std::string>, Grammar_type> Grammar::get_bad_non_term() {
    std::set<std::string> empty_non_term_first;
    std::set<std::string> empty_non_term_second;
    Grammar_type bad_rules;
    
    do {
        empty_non_term_first = empty_non_term_second;
        
        for (Grammar_type::iterator iter = items.begin(); iter!= items.end(); iter++) {
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

void Grammar::proccess_bad_rule(const Rule_type &r, const std::set<std::string> &bad_non_term, Grammar_type &new_items) {
    size_t pos = 0;
    std::for_each(r.second.begin(), r.second.end(), 
    [&](char c)
    {
        if (bad_non_term.contains(get_str_from_char(c))) {
            new_items.insert(r);

            std::string temp(r.second);
            temp.erase(temp.find(c, pos), 1);
            pos++;
            if (temp != "") {
                new_items.insert(std::make_pair(r.first, temp));
                proccess_bad_rule(std::make_pair(r.first, temp), bad_non_term, new_items);
            }
        }

    });
}

void Grammar::break_rules(std::set<std::string> &bad_non_term, Grammar_type bad_rules) {
    Grammar_type new_items;

    std::for_each(items.begin(), items.end(),
    [&](const Rule_type &r) 
    {
        if (bad_rules.contains(r)) {
            if (r.first == "S") {
                new_items.insert(std::make_pair("T", "S"));
                new_items.insert(std::make_pair("T", ""));
            }
            
            if (r.second != "") {
                proccess_bad_rule(r, bad_non_term, new_items);
            } 
            //else rule like (T->eplsilon), don`t add
                
        } else {
            //we need check that right side don`t have bad non term
            if (std::any_of(r.second.begin(), r.second.end(),
            [&](char c){
                if (bad_non_term.contains(get_str_from_char(c))) {
                    return true;
                }
                return false;
            })
            )
            {
                proccess_bad_rule(r, bad_non_term, new_items);
            } else {
                new_items.insert(r);
            }
        }
    }
    );
    
    items = new_items;
}

void Grammar::delete_simple_rules() {
    Grammar_type new_items;

    std::for_each(items.begin(), items.end(),
    [&](const Rule_type &r) 
    {
        if (r.first != r.second) {
            new_items.insert(r);
        }
    }
    );
    
    items = new_items;
}

void Grammar::barren() {
    Grammar_type new_items;

    std::set<std::string> non_term_first;
    std::set<std::string> non_term_second;

    do {
        non_term_first = non_term_second;
        
        for (Grammar_type::iterator iter = items.begin(); iter!= items.end(); iter++) {
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

    std::for_each(items.begin(), items.end(),
    [&](const Rule_type &r) 
    {
        if (non_term_first.find(r.first) != non_term_first.end()) {
            if (std::all_of(r.second.begin(), r.second.end(),
            [&](char c)
            {
                bool is_term = !std::isupper(c);
                return (non_term_first.find(get_str_from_char(c)) != non_term_first.end()) || is_term;
            }))
            {
                new_items.insert(r);
            }
        }
    }
    );

    items = new_items;
}

void Grammar::unattainable() {
    Grammar_type new_items;

    std::set<std::string> non_term_first;
    std::set<std::string> non_term_second;
    
    if (items.find({"T", ""}) != items.end()) {
        non_term_second.insert("T");
    } else {
        non_term_second.insert("S");
    }

    bool empty_too = false;

    do {
        non_term_first = non_term_second;
        for (Grammar_type::iterator iter = items.begin(); iter!= items.end(); iter++) {
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

    std::for_each(items.begin(), items.end(),
    [&](const Rule_type &r) 
    {
        if (non_term_first.find(r.first) != non_term_first.end()) {
            if (std::all_of(r.second.begin(), r.second.end(),
            [&](char c)
            {
                return (non_term_first.find(get_str_from_char(c)) != non_term_first.end());
            }))
            {
                new_items.insert(r);
            }
            if (r.second.empty()) {
                if (empty_too) {
                    new_items.insert(r);
                }
            }
        }
    }
    );

    items = new_items;
}

void Grammar::cast() {
    //Step 1
    std::pair<std::set<std::string>, Grammar_type> temp = get_bad_non_term();
    std::set<std::string> bad_non_term = temp.first;
    Grammar_type bad_rules = temp.second;

    //Step 2, 3
    break_rules(bad_non_term, bad_rules);

    //Remove barren (бесплодные) syms
    barren();
    
    //Remove unattainable(недостижимые) syms
    unattainable();

    //Remove rules like X->X
    delete_simple_rules();
}

void Grammar::print() const {
    std::for_each(items.begin(), items.end(),
    [](const Rule_type &r) 
    {
        std::cout << r.first << ":" << r.second << std::endl;
    }
    );
}

} 

int main() {
    solution::Grammar g(std::cin);
    g.cast();
    g.print();
}