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
        std::make_pair('S',std::string("aS_")), 
        std::make_pair('S',std::string("Sb_")),
        std::make_pair('S',std::string("a"))
    };
}

int main() {
    std::set<std::pair<char, std::string>>  a = g3();
    std::for_each(a.begin(), a.end(), [](const auto &p) {std::cout << p.first << ' ' << p.second << std::endl;} );
    a = g4();
    std::for_each(a.begin(), a.end(), [](const auto &p) {std::cout << p.first << ' ' << p.second << std::endl;} );
}