#include <iostream>
#include <set>
#include <algorithm>

using Rule = std::pair<char, std::string>;
using Grammar = std::set<Rule>;

// G1:
// S → Sabc | d

// G2:
// S → aS | Sb | a



Grammar g3() {
    return {{'S', "Bc"},
            {'S', "d"},
            {'B', "Ab"},
            {'A', "Sa"}};
}


Grammar g4() {
    return {{'S', "A_"},
            {'S', "B_"},
            {'A', "Aa"},
            {'A', "a"},
            {'B', "Bb"},
            {'B', "Ab"}};
}
