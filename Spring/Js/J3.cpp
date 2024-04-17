#include <iostream>
#include <string>
#include <set>

std::set<std::pair<std::string, std::string>> g() {
    std::set<std::pair<std::string, std::string>> ans;
    ans.insert(std::make_pair("S","xSx"));
    ans.insert(std::make_pair("S","ySy"));
    ans.insert(std::make_pair("S","xx"));
    ans.insert(std::make_pair("S","yy"));
    return ans;
}
