#include <iostream>
#include <exception>
#include <set>
#include <list>
#include <map>
#include <algorithm>

std::set<std::string> generate
    (const std::multimap<std::string, std::string> &g, const std::string &start, size_t maxlen) {
    for(std::multimap<std::string, std::string>::const_iterator it = g.begin(); it != g.end(); ++it) {
        if (it->first.empty() || (it->second.length() < it->first.length())) {
            throw std::exception();
        }
    }
    std::set<std::string> finished_strs;
    std::set<std::string> check_cycle_set;
    std::list<std::string> not_finished_strs;
    not_finished_strs.push_back(start);
    while(!not_finished_strs.empty()) {
        bool is_finish = true;
        std::string str = not_finished_strs.front();
        not_finished_strs.pop_front();
        if (str.length() <= maxlen) {
            for(std::multimap<std::string, std::string>::const_iterator it = g.begin(); it != g.end(); ++it) {
                size_t pos = str.find(it->first);
                if (pos != std::string::npos) {
                    is_finish = false;
                    std::string new_str = str.substr(0, pos) + it->second + str.substr(pos + it->first.length());
                    if (check_cycle_set.find(new_str) == check_cycle_set.end() && new_str != str) {
                        check_cycle_set.insert(new_str);
                        not_finished_strs.push_back(new_str);
                    }
                }
            }
            if (is_finish) {
                finished_strs.insert(str);
            }
        }
    }
    return finished_strs;
}
