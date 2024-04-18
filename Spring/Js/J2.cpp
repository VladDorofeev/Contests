#include <iostream>
#include <set>
#include <string>
#include <algorithm>
//NON TERM - ABCDEF
//TERM     - abcdef

typedef std::pair<std::string, std::string> rule; 
typedef std::set<rule> grammar;


//Check all left side of rules in grammar is "T"
bool is_left_non_term(const grammar &g); 

//Check that string like "abcT" or "Tabc"
bool is_regular_string(const std::string &r, bool is_left_side);

//Check all left side of rules in grammar is not shorter than right sides
bool is_not_short(const grammar &g);

//Check that we have rule S->epsilon
bool is_start_to_empty(const grammar &g);

//Check that we have rule that contain start in right 
bool is_start_in_right(const grammar &g);

// All check_type function return:
// True, if they change type
// False, else

bool check_type_0(const grammar &g, int &type) {
    if (std::all_of(g.begin(), g.end(), 
        [](const rule &r) 
        {
            return std::any_of(r.first.begin(), r.first.end(), 
                [](char c) 
                {
                    return isupper(c);
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

bool check_type_3(const grammar &g, int &type) {
    if (is_left_non_term(g) == false) {
        return false;
    }

    //Check left-side grammar
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

    //Check right-side grammar
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

bool check_type_2(const grammar &g, int &type) {
    if (is_left_non_term(g)) {
        type = 2;
        return true;
    }
    return false;
}

bool check_type_1(const grammar &g, int &type) {
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


int grammar_type(const grammar &g) {
    if (g.empty()) {
        return -1;
    }

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

bool is_not_short(const grammar &g) {
    return std::all_of(g.begin(), g.end(), 
        [](const rule &r) 
        {
            return (r.first.length() <= r.second.length()) || 
                ((r.first == "S") && (r.second == ""));
        }
    );
}

bool is_start_to_empty(const grammar &g) {
    return std::any_of(g.begin(), g.end(), 
        [](const rule &r) 
        {
            return (r.first == "S") && (r.second == "");
        }
    );
}

bool is_start_in_right(const grammar &g) {
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
        std::string::const_iterator end_iter = --r.end();
        return std::all_of(r.begin(), end_iter, 
            [](char c) 
            {
                return !isupper(c);
            }
        );
    } 

    //RIGHT-SIDE
    std::string::const_iterator begin_iter = ++r.begin();
    return std::all_of(begin_iter, r.end(), 
        [](char c) 
        {
            return !isupper(c);
        }
    );
}

bool is_left_non_term(const grammar &g) {
    return std::all_of(g.begin(), g.end(), 
        [](const rule &r)
        {
            bool left_is_once_non_term = (r.first.length() == 1) && (isupper(r.first[0]));
            return left_is_once_non_term;
        }
    );
}

#ifdef _main
//0
std::set<std::pair<std::string, std::string>> g0() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("T","Ta"));
    ans.insert(std::make_pair("S","a"));
    return ans;
}

//1
// S → 0A1
// A → 0A0
// A →
std::set<std::pair<std::string, std::string>> g1() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","0A1"));
    ans.insert(std::make_pair("A","0A0"));
    ans.insert(std::make_pair("A",""));
    return ans;
}
//2
//S → aSb | 
std::set<std::pair<std::string, std::string>> g2() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","aSb"));
    ans.insert(std::make_pair("S",""));
    return ans;
}
//3
// S → 0A
// A → 0B | 1
// B → 0A
std::set<std::pair<std::string, std::string>> g3() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","0A"));
    ans.insert(std::make_pair("A","0B"));
    ans.insert(std::make_pair("A","1"));
    ans.insert(std::make_pair("B","0A"));
    return ans;
}
//4
// S → aA | a |
// А → a | aA
std::set<std::pair<std::string, std::string>> g4() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","aA"));
    ans.insert(std::make_pair("S","a"));
    ans.insert(std::make_pair("S",""));
    ans.insert(std::make_pair("A","a"));
    ans.insert(std::make_pair("A","aA"));
    return ans;
}
//5
// S → A | B
// A → a | Ba
// B → b | Bb | Ab
std::set<std::pair<std::string, std::string>> g5() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","A"));
    ans.insert(std::make_pair("S","B"));
    ans.insert(std::make_pair("A","a"));
    ans.insert(std::make_pair("A","Ba"));
    ans.insert(std::make_pair("B","b"));
    ans.insert(std::make_pair("B","Bb"));
    ans.insert(std::make_pair("B","Ab"));
    return ans;
}
//6
// S → aQb | accb
// Q → cSc
std::set<std::pair<std::string, std::string>> g6() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","aQb"));
    ans.insert(std::make_pair("S","accb"));
    ans.insert(std::make_pair("Q","cSc"));
    return ans;
}
//7
// S → aSa | bSb | 
std::set<std::pair<std::string, std::string>> g7() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","aSa"));
    ans.insert(std::make_pair("S","bSb"));
    ans.insert(std::make_pair("S",""));
    return ans;
}
//8
// S → aSBC | abC
// CB → BC
// bB → bb
// bC → bc
// cC → cc
std::set<std::pair<std::string, std::string>> g8() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","aSBC"));
    ans.insert(std::make_pair("S","abC"));
    ans.insert(std::make_pair("CB","BC"));
    ans.insert(std::make_pair("bB","bb"));
    ans.insert(std::make_pair("bC","bc"));
    ans.insert(std::make_pair("cC","cc"));
    return ans;
}
//9
// S → aSBC | abC
// CB → CD
// CD → BD
// BD → BC
// bB → bb
// bC → bc
// cC → cc
std::set<std::pair<std::string, std::string>> g9() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","aSBC"));
    ans.insert(std::make_pair("S","abC"));
    ans.insert(std::make_pair("CB","CD"));
    ans.insert(std::make_pair("CD","BD"));
    ans.insert(std::make_pair("BD","BC"));
    ans.insert(std::make_pair("bB","bb"));
    ans.insert(std::make_pair("bC","bc"));
    ans.insert(std::make_pair("cC","cc"));
    return ans;
}
//10
// S → SS
// SS → Sa | S
std::set<std::pair<std::string, std::string>> g10() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","SS"));
    ans.insert(std::make_pair("SS","Sa"));
    ans.insert(std::make_pair("SS","S"));
    return ans;
}
//11
// S → SS
// SS → 
std::set<std::pair<std::string, std::string>> g11() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","SS"));
    ans.insert(std::make_pair("SS",""));
    return ans;
}

int
main()
{
    std::cout << "g(0) = " << (grammar_type(g0())) << std::endl;  
    std::cout << (grammar_type(g1()) == 2) << std::endl;  
    std::cout << (grammar_type(g2()) == 2) << std::endl;  
    std::cout << (grammar_type(g3()) == 3) << std::endl;  
    std::cout << (grammar_type(g4()) == 3) << std::endl;  
    std::cout << (grammar_type(g5()) == 3) << std::endl;  
    std::cout << (grammar_type(g6()) == 2) << std::endl;  
    std::cout << (grammar_type(g7()) == 2) << std::endl; 
    std::cout << (grammar_type(g8()) == 1) << std::endl;  
    std::cout << (grammar_type(g9()) == 1) << std::endl; 
    std::cout << (grammar_type(g10()) == 0) << std::endl;  
    std::cout << (grammar_type(g11()) == 0) << std::endl; 
}
#endif