#include <iostream>
#include <set>
#include <string>
#include <algorithm>

std::set<std::pair<char, std::string>> g3() {

    return {
        std::make_pair('S',std::string("Cc")), 
        std::make_pair('S',std::string("d")),
        std::make_pair('C',std::string("Bb")),
        std::make_pair('B',std::string("Sa")),
    };
}

std::set<std::pair<char, std::string>> g4() {

    return {
        std::make_pair('S',std::string("A_")), 
        std::make_pair('S',std::string("B_")), 
        std::make_pair('B',std::string("Ab")), 
        std::make_pair('B',std::string("Bb")), 
        std::make_pair('A',std::string("a")), 
        std::make_pair('A',std::string("Aa")), 
    };
}