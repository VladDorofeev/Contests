#include <iostream>
#include <set>
#include <string>
#include <algorithm>

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
    // std::cout << "parse r " << r.first << ":" << r.second << std::endl;
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
        // std::cout << "rule " << r.first << ':' << r.second << std::endl;

        if (bad_rules.contains(r)) {
            if (r.first == "S") {
                new_items.insert(std::make_pair("T", "S"));
                new_items.insert(std::make_pair("T", ""));
                // std::cout << "left side S, added new rule" << std::endl;
            }
            
            if (r.second != "") {
                // std::cout << "not empty bad rule, go procces " << std::endl;
                proccess_bad_rule(r, bad_non_term, new_items);
            } else {
                //else rule like (T->eplsilon), don`t add
                // std::cout << "bad rule, because empty, skip "  << std::endl;
            }
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
                // std::cout << "left gut, right have bad nonterm, go procces "  << std::endl;
                proccess_bad_rule(r, bad_non_term, new_items);
            } else {
                // std::cout << "normal rule, just added "  << std::endl;
                new_items.insert(r);
            }
        }
    }
    );
    // std::cout << std::endl;
    
    items = new_items;
}


void Grammar::cast() {
    std::pair<std::set<std::string>, Grammar_type> temp = get_bad_non_term();

    std::set<std::string> bad_non_term = temp.first;
    Grammar_type bad_rules = temp.second;

    break_rules(bad_non_term, bad_rules);

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