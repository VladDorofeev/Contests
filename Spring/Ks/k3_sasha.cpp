#include <iostream>
#include <set>


using Rule = std::pair<char, std::string>;
using Grammar = std::set<Rule>;


// G1:
// S → aS | bS | a

// G2:
// S → Sa | Ab | a   //a..aaaa..ababaaaab...
// A → Sa


Grammar g3() {
    return {{'S', "Sa"},
            {'S', "Aa"},
            {'S', "a"},
            {'A', "b"},
            {'A', "Sb"},
            {'A', "Ab"}};
}


Grammar g4() {
    return {{'S', "aA"},
            {'A', "aB"},
            {'A', "_"},
            {'B', "bA"},
            {'B', "aB"},
            {'B', "_"}};
}