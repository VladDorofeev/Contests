#include <iostream>
#include <set>


using Rule = std::pair<char, std::string>;
using Grammar = std::set<Rule>;

// L1:
// {c (a|b)n, n ≥ 0}

// L2:
// {(a|b)n c, n ≥ 0}



Grammar g1() {
    return {{'S', "cA"},
            {'A', "aA"},
            {'A', "bA"},
            {'A', "_"}};
}


Grammar g2() {
    return {{'S', "aS"},
            {'S', "bS"},
            {'S', "c"}};
}