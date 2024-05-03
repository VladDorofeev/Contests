#include <iostream>
#include <set>
#include <string>
#include <algorithm>
#include <map>

typedef std::pair<std::string, std::string> Rule; 
// typedef std::pair<char, std::string> Rule;
typedef std::set<Rule> Unreduced_Grammar;
typedef std::map<char, std::set<std::string>> Grammar;

namespace type_check
{
    #include <iostream>
#include <set>
#include <string>
#include <algorithm>
//NON TERM - ABCDEF
//TERM     - abcdef

typedef std::pair<std::string, std::string> rule; 
typedef std::set<rule> Unreduced_Grammar;

//Check all left side of rules in Unreduced_Grammar is "T"
bool is_left_non_term(const Unreduced_Grammar &g); 

//Check that string like "abcT" or "Tabc"
bool is_regular_string(const std::string &r, bool is_left_side);

//Check all left side of rules in Unreduced_Grammar is not shorter than right sides
bool is_not_short(const Unreduced_Grammar &g);

//Check that we have rule S->epsilon
bool is_start_to_empty(const Unreduced_Grammar &g);

//Check that we have rule that contain start in right 
bool is_start_in_right(const Unreduced_Grammar &g);

// All check_type function return:
// True, if they change type
// False, else

bool check_type_0(const Unreduced_Grammar &g, int &type) {
    if (std::all_of(g.begin(), g.end(), 
        [](const rule &r) 
        {
            return std::any_of(r.first.begin(), r.first.end(), 
                [](char c) 
                {
                    return std::isupper(c);
                }
            );
        })
    ) 
    {
        type = 0;
        return true;
    }
    return false;
}

bool check_type_3(const Unreduced_Grammar &g, int &type) {
    if (is_left_non_term(g) == false) {
        return false;
    }

    //Check left-side Unreduced_Grammar
    if (std::all_of(g.begin(), g.end(), 
        [](const rule &r)
        {
            return is_regular_string(r.second, true);
        }
    ))
    {
        type = 3;
        return true;
    }

    //Check right-side Unreduced_Grammar
    if (std::all_of(g.begin(), g.end(), 
        [](const rule &r)
        {
            return is_regular_string(r.second, false);
        }
    ))
    {
        type = 3;
        return true;
    } 

    return false;
}

bool check_type_2(const Unreduced_Grammar &g, int &type) {
    if (is_left_non_term(g)) {
        type = 2;
        return true;
    }
    return false;
}

bool check_type_1(const Unreduced_Grammar &g, int &type) {
    if (is_start_to_empty(g)) {
        if (is_start_in_right(g)) {
            return false;
        }
    }
    if (is_not_short(g)) {
        type = 1;
        return true;
    }
    return false;
}

int grammar_type(const Unreduced_Grammar &g) {
    int type = -1;
    
    if (check_type_0(g, type) == false) {
        return type;
    }

    if (check_type_3(g, type)) {
        return type;
    }

    if (check_type_2(g, type)) {
        return type;
    }

    if (check_type_1(g, type)) {
        return type;
    }

    return type;
}

bool is_not_short(const Unreduced_Grammar &g) {
    return std::all_of(g.begin(), g.end(), 
        [](const rule &r) 
        {
            return (r.first.length() <= r.second.length()) || 
                ((r.first == "S") && (r.second == ""));
        }
    );
}

bool is_start_to_empty(const Unreduced_Grammar &g) {
    return std::any_of(g.begin(), g.end(), 
        [](const rule &r) 
        {
            return (r.first == "S") && (r.second == "");
        }
    );
}

bool is_start_in_right(const Unreduced_Grammar &g) {
    return std::any_of(g.begin(), g.end(), 
        [](const rule &r) 
        {
            return r.second.find("S") != std::string::npos;
        }
    );
}


bool is_regular_string(const std::string &r, bool is_left_side) {
    if (r.empty()) {
        return true;
    }
    //LEFT-SIDE
    if (is_left_side) {
        return std::all_of(r.begin(), --r.end(), 
            [](char c) 
            {
                return !std::isupper(c);
            }
        );
    } 

    //RIGHT-SIDE
    return std::all_of(++r.begin(), r.end(), 
        [](char c) 
        {
            return !std::isupper(c);
        }
    );
}

bool is_left_non_term(const Unreduced_Grammar &g) {
    return std::all_of(g.begin(), g.end(), 
        [](const rule &r)
        {
            bool left_is_once_non_term = (r.first.length() == 1) && (std::isupper(r.first[0]));
            return left_is_once_non_term;
        }
    );
}
} // namespace type_check

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void grammar_create(Unreduced_Grammar &grammar, std::istream &in) 
{
    std::string s;
    while (std::getline(in, s)) {
        std::size_t pos = s.find(":");
        grammar.insert(std::make_pair(s.substr(0,pos), s.substr(pos+1, s.length())));
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

Grammar reduced(const Unreduced_Grammar &g) {
    Grammar new_g;
    std::for_each(g.begin(), g.end(), 
    [&](const Rule &r)
    {
        new_g[r.first[0]].insert(r.second);
    });
    return new_g;
}

bool cap(const std::set<char> &s1, const std::set<char> &s2) {
    bool is_cap = false;
    std::for_each(s2.begin(), s2.end(),
    [&](char c) {
        if (s1.find(c) != s1.end()) {
            is_cap = true;
        }
    });
    return is_cap;
}

std::set<char> operator+ (const std::set<char> &s1, const std::set<char> &s2) {
    std::set<char> res = s1;
    std::for_each(s2.begin(), s2.end(),
    [&](char c) {
        res.insert(c);
    });
    return res;
}

std::set<char> get_first(Grammar &g, char nonterm, std::set<char> &stack, std::set<char> &empty_nonterm) {
    std::set<char> firsts;
    std::for_each(g[nonterm].begin(), g[nonterm].end(),
    [&](const std::string &s) {
        if (s.length() > 0) {
            if (!std::isupper(s[0])) {
                if (cap(firsts, {s[0]})) {
                    throw nonterm;
                }
                firsts.insert(s[0]);
            } else {
                if (stack.find(s[0]) == stack.end()) {
                    stack.insert(s[0]);
                    std::set<char> temp = get_first(g, s[0], stack, empty_nonterm);
                    stack.erase(s[0]);
                    
                    if (empty_nonterm.find(s[0]) != empty_nonterm.end()) {
                        std::set<char> temp2;
                        if (!std::isupper(s[1])) {
                            temp2.insert(s[1]);
                        } else {
                            stack.insert(s[1]);
                            temp2 = get_first(g, s[1], stack, empty_nonterm); 
                            stack.erase(s[1]);
                        }

                        if (cap(temp, temp2)) {
                            throw nonterm;
                        }
                        temp = temp + temp2;
                    }
                    if (cap(firsts, temp)) {
                        throw nonterm;
                    }
                    firsts = firsts + temp;
                }
            }
        }
    });
    return firsts;
}
std::pair<std::set<char>, std::set<std::string>> get_bad_non_term(Unreduced_Grammar &grammar) {
    std::set<char> empty_non_term_first;
    std::set<char> empty_non_term_second;
    std::set<std::string> bad_rules;
    do {
        empty_non_term_first = empty_non_term_second;
        
        for (Unreduced_Grammar::iterator iter = grammar.begin(); iter!= grammar.end(); iter++) {
            if (std::all_of(iter->second.begin(), iter->second.end(),
            [&](char c)
            {
                return (empty_non_term_first.find(c) != empty_non_term_first.end());
            }))
            {
                bad_rules.insert(iter->second);
                empty_non_term_second.insert(iter->first[0]);
            }
        }
    } while (empty_non_term_first != empty_non_term_second);
    
    return std::make_pair(empty_non_term_first, bad_rules);
}

std::set<char> get_nonterms(const Grammar &g) {
    std::set<char> nonterms;      
    for (const auto &pair:g) {
        nonterms.insert(pair.first);
    }
    return nonterms;
}

bool check_first_first(Grammar &g, std::map<char, std::set<char>> &_firsts, std::set<char> &empty_nonterm) {
    bool is_good = true;
    std::set<char> nonterms = get_nonterms(g);
    std::map<char, std::set<char>> firsts;
    
    std::for_each(nonterms.begin(), nonterms.end(), 
    [&](char nonterm)
    {
        try
        {
            std::set<char> call_stack;
            firsts[nonterm] = get_first(g, nonterm, call_stack, empty_nonterm);
        }
        catch(char c)
        {
            is_good = false;
        }
    });
    _firsts = firsts;
    return is_good;
}


bool check_eps(Grammar &g, Unreduced_Grammar unred_g, std::set<char> &_empty_nonterm) {
    std::pair<std::set<char>, std::set<std::string>> temp = get_bad_non_term(unred_g); 
    std::set<char> empty_nonterm = temp.first;
    std::set<std::string> bad_rules = temp.second;

    for (char nonterm : empty_nonterm) {
        int cnt_bad_rules = 0;
        for (const std::string &s : g[nonterm]) {
            if (bad_rules.find(s) != bad_rules.end()) {
                cnt_bad_rules++;
            }
        }
        if (cnt_bad_rules > 1) {
            return false;
        }
        cnt_bad_rules = 0;
    }
    _empty_nonterm = empty_nonterm;
    return true;
}

std::set<char> get_follow(
        Grammar &g, 
        char nonterm, 
        std::map<char, std::set<char>> &firsts, 
        std::set<char> &empty_nonterms
    ) 
{
    std::set<char> follow;
    for (const auto &pair : g) {
        if (pair.first != nonterm) {
            std::for_each(pair.second.begin(), pair.second.end(),
            [&](const std::string &s) 
            {
                if (s.length() > 0) {
                    size_t pos = s.find(nonterm);
                    if (pos != std::string::npos) {
                        if ((pos + 1) >= s.length()) {
                            follow = follow + get_follow(g, pair.first, firsts, empty_nonterms);
                        } else {
                            if (s.at(pos + 1) != nonterm) {
                                if (!std::isupper(s.at(pos+1))) {
                                    follow.insert(s.at(pos + 1));
                                } else {
                                    if (empty_nonterms.find(s.at(pos + 1)) != empty_nonterms.end()) {
                                        follow = follow + get_follow(g, pair.first, firsts, empty_nonterms);
                                    }
                                    follow = follow + firsts[s.at(pos + 1)];
                                }
                            }
                        }
                    }
                }
            });
        }
    }
    if (cap(follow, firsts[nonterm])) {
        throw nonterm;
    }
    return follow;
}

bool check_first_follow(Grammar &g, std::map<char, std::set<char>> firsts, std::set<char> &empty_nonterm) {
    bool is_good = true;
    std::map<char, std::set<char>> follows;
    std::for_each(empty_nonterm.begin(), empty_nonterm.end(), 
    [&](char nonterm)
    {
        try
        {
            follows[nonterm] = get_follow(g, nonterm, firsts, empty_nonterm);
        }
        catch(char c)
        {
            is_good = false;
        }
    });
    return is_good;
}

bool is_rd(Unreduced_Grammar g) {
    if (type_check::grammar_type(g) < 2) {
        return false;
    }
    Grammar reduced_g = reduced(g);
    std::map<char, std::set<char>> firsts;

    std::pair<std::set<char>, std::set<std::string>> temp = get_bad_non_term(g); 
    std::set<char> empty_nonterm = temp.first;
    std::set<std::string> bad_rules = temp.second;

    return 
        check_first_first(reduced_g, firsts, empty_nonterm) && 
        check_eps(reduced_g, g, empty_nonterm) && 
        check_first_follow(reduced_g, firsts, empty_nonterm);
}

int main() {
    Unreduced_Grammar g;
    grammar_create(g, std::cin);
    std::cout << (is_rd(g) ? "YES" : "NO") << std::endl;
    return 0;
}