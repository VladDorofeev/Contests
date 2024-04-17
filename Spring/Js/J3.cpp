#include <iostream>
#include <string>
#include <set>

std::set<std::pair<std::string, std::string>> g() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","xBx"));
    ans.insert(std::make_pair("S","yBy"));
    ans.insert(std::make_pair("B",""));
    ans.insert(std::make_pair("B","S"));
    return ans;
}