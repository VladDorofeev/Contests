#include <iostream>
#include <set>
#include <string>
#include <algorithm>

std::set<std::pair<char, std::string>> g1() {

    return {
        std::make_pair('S',std::string("cA")), 
        std::make_pair('A',std::string("aA")),
        std::make_pair('A',std::string("bA")),
        std::make_pair('A',std::string("_"))
    };
}

std::set<std::pair<char, std::string>> g2() {

    return {
        std::make_pair('S',std::string("aS")), 
        std::make_pair('S',std::string("bS")), 
        std::make_pair('S',std::string("c")) 
    };
}