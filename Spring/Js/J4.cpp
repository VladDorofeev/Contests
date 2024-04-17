#include <iostream>
#include <set>

std::set<std::pair<std::string, std::string>> g() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert({"S", "aAB"});
    ans.insert({"S", "E"});
    ans.insert({"A", "dDA"});
    ans.insert({"A", ""});
    ans.insert({"B", "bE"});
    ans.insert({"B", "f"});
    ans.insert({"D", "eA"});
    ans.insert({"E", "fA"});
    ans.insert({"E", "g"});
    return ans;
}