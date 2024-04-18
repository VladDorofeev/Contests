#include <iostream>
#include <set>
#include <algorithm>

using Grammar = std::set<std::pair<std::string, std::string>>;
using Rule = std::pair<std::string, std::string>;

bool exist_start_sym(Rule const& rule) {
    if (rule.first == "S") {
        return true;
    }
    return false;
}


bool empty_start(Rule const& rule) {
    if (rule.first == "S" && rule.second == "") {
        return true;
    }
    return false;
}


bool contain_empty_start(Rule const& rule) {
    if (rule.second.find("S") != std::string::npos) {
        return true;
    }
    return false;
}

bool has_non_terminal_left_rule(Rule const& rule) {
    for(std::string::size_type i = 0; i < rule.first.length(); ++i) {
        if (std::isupper(rule.first[i])) {
            return true;
        }
    }
    return false;
}


bool has_only_one_non_terminal_left(Rule const& rule) {
    if (rule.first.length() == 1 && std::isupper(rule.first[0])) {
        return true;
    }
    return false;
}


bool all_left_is_terminal(Rule const& rule) {
    if (rule.second.length() == 0) {
        return true;
    }
    for(std::string::size_type i = 0; i < rule.second.length() - 1; ++i) {
        if (std::isupper(rule.second[i])) {
            return false;
        }
    }
    return true;
}


bool all_right_is_terminal(Rule const& rule) {
    if (rule.second.length() == 0) {
        return true;
    }
    for(std::string::size_type i = 1; i < rule.second.length(); ++i) {
        if (std::isupper(rule.second[i])) {
            return false;
        }
    }
    return true;
}


bool all_right_more_or_equal_left(Rule const& rule) {
    if ((rule.first.length() <= rule.second.length()) || (rule.first == "S" && rule.second == "")) {
        return true;
    }
    return false;
}

bool is_grammar(Grammar const& g) {
    return std::all_of(g.begin(), g.end(), has_non_terminal_left_rule);
}


bool is_regular_left(Grammar const& g) {
    return std::all_of(g.begin(), g.end(), all_right_is_terminal);
}


bool is_regular_right(Grammar const& g) {
    return std::all_of(g.begin(), g.end(), all_left_is_terminal);
}


bool is_not_short(Grammar const& g) {
    if (std::any_of(g.begin(), g.end(), empty_start)) {
        if (std::any_of(g.begin(), g.end(), contain_empty_start)) {
            return false;
        }
    }
    return std::all_of(g.begin(), g.end(), all_right_more_or_equal_left);
}


int grammar_type(Grammar const& g) {
    if (is_grammar(g)) {
        if (std::all_of(g.begin(), g.end(), has_only_one_non_terminal_left)) {
            if (is_regular_left(g) || is_regular_right(g)) {
                return 3;
            }
            return 2;
        }
        if (is_not_short(g)) {
            return 1;
        }
        return 0;
    }
    return -1;
}
// //1
// // S → 0A1
// // A → 0A0
// // A →
// std::set<std::pair<std::string, std::string>> g1() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","0A"));
//     ans.insert(std::make_pair("S","0"));
//     ans.insert(std::make_pair("S",""));
//     return ans;
// }
// //2
// //S → aSb | 
// std::set<std::pair<std::string, std::string>> g2() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","aSb"));
//     ans.insert(std::make_pair("S",""));
//     return ans;
// }
// //3
// // S → 0A
// // A → 0B | 1
// // B →0A
// std::set<std::pair<std::string, std::string>> g3() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","0A"));
//     ans.insert(std::make_pair("A","0B"));
//     ans.insert(std::make_pair("A","1"));
//     ans.insert(std::make_pair("B","0A"));
//     return ans;
// }
// //4
// // S → aA | a |
// // А → a | aA
// std::set<std::pair<std::string, std::string>> g4() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","aA"));
//     ans.insert(std::make_pair("S","a"));
//     ans.insert(std::make_pair("S",""));
//     ans.insert(std::make_pair("A","a"));
//     ans.insert(std::make_pair("A","aA"));
//     return ans;
// }
// //5
// // S → A | B
// // A → a | Ba
// // B → b | Bb | Ab
// std::set<std::pair<std::string, std::string>> g5() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","A"));
//     ans.insert(std::make_pair("S","B"));
//     ans.insert(std::make_pair("A","a"));
//     ans.insert(std::make_pair("A","Ba"));
//     ans.insert(std::make_pair("B","b"));
//     ans.insert(std::make_pair("B","Bb"));
//     ans.insert(std::make_pair("B","Ab"));
//     return ans;
// }
// //6
// // S → aQb | accb
// // Q → cSc
// std::set<std::pair<std::string, std::string>> g6() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","aQb"));
//     ans.insert(std::make_pair("S","accb"));
//     ans.insert(std::make_pair("Q","cSc"));
//     return ans;
// }
// //7
// // S → aSa | bSb | 
// std::set<std::pair<std::string, std::string>> g7() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","aSa"));
//     ans.insert(std::make_pair("S","bSb"));
//     ans.insert(std::make_pair("S",""));
//     return ans;
// }
// //8
// // S → aSBC | abC
// // CB → BC
// // bB → bb
// // bC → bc
// // cC → cc
// std::set<std::pair<std::string, std::string>> g8() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","aSBC"));
//     ans.insert(std::make_pair("S","abC"));
//     ans.insert(std::make_pair("CB","BC"));
//     ans.insert(std::make_pair("bB","bb"));
//     ans.insert(std::make_pair("bC","bc"));
//     ans.insert(std::make_pair("cC","cc"));
//     return ans;
// }
// //9
// // S → aSBC | abC
// // CB → CD
// // CD → BD
// // BD → BC
// // bB → bb
// // bC → bc
// // cC → cc
// std::set<std::pair<std::string, std::string>> g9() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","aSBC"));
//     ans.insert(std::make_pair("S","abC"));
//     ans.insert(std::make_pair("CB","CD"));
//     ans.insert(std::make_pair("CD","BD"));
//     ans.insert(std::make_pair("BD","BC"));
//     ans.insert(std::make_pair("bB","bb"));
//     ans.insert(std::make_pair("bC","bc"));
//     ans.insert(std::make_pair("cC","cc"));
//     return ans;
// }
// //10
// // S → SS
// // SS → Sa | S
// std::set<std::pair<std::string, std::string>> g10() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","SS"));
//     ans.insert(std::make_pair("SS","Sa"));
//     ans.insert(std::make_pair("SS","S"));
//     return ans;
// }
// //11
// // S → SS
// // SS → 
// std::set<std::pair<std::string, std::string>> g11() {
//     std::set<std::pair<std::string, std::string>> ans;
//     ans.insert(std::make_pair("S","SS"));
//     ans.insert(std::make_pair("SS",""));
//     return ans;
// }

// int
// main()
// {
//    std::cout << grammar_type(g1()) << std::endl;  
//    std::cout << grammar_type(g2()) << std::endl;  
//    std::cout << grammar_type(g3()) << std::endl;  
//      std::cout << grammar_type(g4()) << std::endl;  
//      std::cout << grammar_type(g5()) << std::endl;  
//      std::cout << grammar_type(g6()) << std::endl;  
//        std::cout << grammar_type(g7()) << std::endl; 
//       std::cout << grammar_type(g8()) << std::endl;  
//        std::cout << grammar_type(g9()) << std::endl; 
//          std::cout << grammar_type(g10()) << std::endl;  
//             std::cout << grammar_type(g11()) << std::endl; 
// }
