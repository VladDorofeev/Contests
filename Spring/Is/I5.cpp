#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

std::set<std::string> 
generate(const std::multimap<std::string, std::string> &g, const std::string &start, size_t maxlen) {
    //Check our map
    for (std::multimap<std::string, std::string>::const_iterator it = g.begin(); it != g.end(); ++it) {
        if (it->first.empty() || (it->second.length() < it->first.length())) {
            throw std::exception();
        }
    }
    
    //Result
    std::set<std::string> end_strs; 
    
    //Helpfull array
    std::vector<std::string> arr;

    //Check cycle conversation
    std::set<std::string> repeat_strs;


    arr.push_back(std::string(start));
    std::vector<std::string>::iterator on_begin;
    while (arr.empty() == false)
    {
        //get first string
        on_begin = arr.begin();
        std::string s = *on_begin;
        arr.erase(on_begin);



        bool ended_str = true;
        if (s.length() <= maxlen) {
            for (std::multimap<std::string, std::string>::const_iterator iter = g.begin(); iter != g.end(); iter++) {
                std::size_t pos = s.find(iter->first);
                if (pos != std::string::npos) {
                    //Not a ended string
                    ended_str = false;

                    //Get a new string
                    std::string new_s = s;
                    new_s.replace(pos, iter->first.length(), iter->second);
                    
                    //Check cycle conversation
                    if ((repeat_strs.find(new_s) == repeat_strs.end()) && (new_s != s)) {
                        repeat_strs.insert(new_s);
                        arr.push_back(new_s);
                    }
                }
            }

            if (ended_str) {
                end_strs.insert(s);
            }
        }
        
    }

    return end_strs;
}



#ifdef _main
int main() {
    std::multimap<std::string, std::string> g;
    g.insert(std::make_pair("a", "c"));
    g.insert(std::make_pair("c", "a"));

    std::string s = "cac";

    std::set<std::string> gen_set = generate(g, s, 100);

    for (std::set<std::string>::iterator iter = gen_set.begin(); iter != gen_set.end(); iter++) {
        std::cout << (*iter) << std::endl;
    }


    return 0;
}
#endif