#include <iostream>
#include <set>
#include <string>
#include <algorithm>

std::set<std::pair<char, std::string>> g3() {

    return {
        std::make_pair('S',std::string("Sa")), 
        std::make_pair('S',std::string("Ba")), 
        std::make_pair('S',std::string("a")), 
        std::make_pair('B',std::string("b")), 
        std::make_pair('B',std::string("Bb")), 
        std::make_pair('B',std::string("Sb"))
    };
}

std::set<std::pair<char, std::string>> g4() {

    return {
        std::make_pair('S',std::string("aL")), 
        std::make_pair('L',std::string("aK")), 
        std::make_pair('K',std::string("aK")), 
        std::make_pair('K',std::string("bL")), 
        std::make_pair('K',std::string("_")), 
        std::make_pair('L',std::string("_")), 
    };
}
