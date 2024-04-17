#include <iostream>
#include <set>
// S → aAB | E
// A → dDA | ε
// B → bE | f
// C → cAB | dSD | a  - С unused
// D → eA
// E → fA | g

std::set<std::pair<std::string, std::string>> g() {
    std::set<std::pair<std::string, std::string>> result;
    result.insert({"S", "aAB"});
    result.insert({"S", "E"});
    result.insert({"A", "dDA"});
    result.insert({"A", ""});
    result.insert({"B", "bE"});
    result.insert({"B", "f"});
    result.insert({"D", "eA"});
    result.insert({"E", "fA"});
    result.insert({"E", "g"});
    return result;
}