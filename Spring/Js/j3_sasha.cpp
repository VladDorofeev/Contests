#include <iostream>
#include <set>

std::set<std::pair<std::string, std::string>> g() {
    std::set<std::pair<std::string, std::string>> result;
    result.insert({"S" , "xx"});
    result.insert({"S" , "yy"});
    result.insert({"S" , "xSx"});
    result.insert({"S" , "ySy"});
    return result;
}