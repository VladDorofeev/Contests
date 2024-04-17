#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

namespace with_vector
{
    int fun(std::string const &s1, std::string const &s2);
} 
namespace with_list
{
    int fun(std::string const &s1, std::string const &s2);
} 
namespace with_best
{
    int fun(std::string const &s1, std::string const &s2);
} 

int with_vector::fun(std::string const &s1, std::string const &s2) {
    if (s1.length() == 0 && s2.length() == 0) {
        return 1;
    }
    std::vector<std::string> unique_strs;
    if (s1.length() == 0) {
        for (std::string::size_type m = 0; m < s2.length(); m++) {
            for (std::string::size_type k = 0; k < s2.length() - m + 1; k++) {
                std::string substr = s2.substr(m,k);

                if (std::count(unique_strs.begin(), unique_strs.end(), substr) == 0) {
                    unique_strs.push_back(substr);
                }
            }
        }
        return unique_strs.size();
    }

    if (s2.length() == 0) {
        for (std::string::size_type i = 0; i < s1.length(); i++) {
            for (std::string::size_type j = 0; j < s1.length() - i + 1; j++) {
                std::string substr = s1.substr(i, j);

                if (std::count(unique_strs.begin(), unique_strs.end(), substr) == 0) {
                    unique_strs.push_back(substr);
                } 
            }
        }
    }

    for (std::string::size_type i = 0; i < s1.length(); i++) {
        for (std::string::size_type j = 0; j < s1.length() - i + 1; j++) {
            for (std::string::size_type m = 0; m < s2.length(); m++) {
                for (std::string::size_type k = 0; k < s2.length() - m + 1; k++) {

                    std::string substr1 = s1.substr(i, j) + s2.substr(m,k);
                    std::string substr2 = s2.substr(m,k) + s1.substr(i, j);

                    if (std::count(unique_strs.begin(), unique_strs.end(), substr1) == 0) {
                        unique_strs.push_back(substr1);
                    }
                    if (std::count(unique_strs.begin(), unique_strs.end(), substr2) == 0) {
                        unique_strs.push_back(substr2);
                    }
                }
            }
        }
    }


    return unique_strs.size();
}

int with_list::fun(std::string const &s1, std::string const &s2) {
    if (s1.length() == 0 && s2.length() == 0) {
        return 1;
    }
    std::list<std::string> unique_strs;
    if (s1.length() == 0) {
        for (std::string::size_type m = 0; m < s2.length(); m++) {
            for (std::string::size_type k = 0; k < s2.length() - m + 1; k++) {
                std::string substr = s2.substr(m,k);

                if (std::count(unique_strs.begin(), unique_strs.end(), substr) == 0) {
                    unique_strs.push_back(substr);
                }
            }
        }
        return unique_strs.size();
    }

    if (s2.length() == 0) {
        for (std::string::size_type i = 0; i < s1.length(); i++) {
            for (std::string::size_type j = 0; j < s1.length() - i + 1; j++) {
                std::string substr = s1.substr(i, j);

                if (std::count(unique_strs.begin(), unique_strs.end(), substr) == 0) {
                    unique_strs.push_back(substr);
                } 
            }
        }
    }

    for (std::string::size_type i = 0; i < s1.length(); i++) {
        for (std::string::size_type j = 0; j < s1.length() - i + 1; j++) {
            for (std::string::size_type m = 0; m < s2.length(); m++) {
                for (std::string::size_type k = 0; k < s2.length() - m + 1; k++) {

                    std::string substr1 = s1.substr(i, j) + s2.substr(m,k);
                    std::string substr2 = s2.substr(m,k) + s1.substr(i, j);

                    if (std::count(unique_strs.begin(), unique_strs.end(), substr1) == 0) {
                        unique_strs.push_back(substr1);
                    }
                    if (std::count(unique_strs.begin(), unique_strs.end(), substr2) == 0) {
                        unique_strs.push_back(substr2);
                    }
                }
            }
        }
    }

    return unique_strs.size();
}

int with_best::fun(std::string const &s1, std::string const &s2) {
    if (s1.length() == 0 && s2.length() == 0) {
        return 1;
    }
    std::set<std::string> unique_strs;
    if (s1.length() == 0) {
        for (std::string::size_type m = 0; m < s2.length(); m++) {
            for (std::string::size_type k = 0; k < s2.length() - m + 1; k++) {
                std::string substr = s2.substr(m,k);

                unique_strs.insert(substr);
            }
        }
        return unique_strs.size();
    }

    if (s2.length() == 0) {
        for (std::string::size_type i = 0; i < s1.length(); i++) {
            for (std::string::size_type j = 0; j < s1.length() - i + 1; j++) {
                std::string substr = s1.substr(i, j);

                unique_strs.insert(substr);
            }
        }
    }

    for (std::string::size_type i = 0; i < s1.length(); i++) {
        for (std::string::size_type j = 0; j < s1.length() - i + 1; j++) {
            for (std::string::size_type m = 0; m < s2.length(); m++) {
                for (std::string::size_type k = 0; k < s2.length() - m + 1; k++) {

                    std::string substr1 = s1.substr(i, j) + s2.substr(m,k);
                    std::string substr2 = s2.substr(m,k) + s1.substr(i, j);

                    unique_strs.insert(substr1);
                    unique_strs.insert(substr2);
                }
            }
        }
    }

    return unique_strs.size();
}



#ifdef _main
int 
main() {
    std::string s1 = "";
    std::string s2 = "";
    std::cout << "LENGTH = " << with_vector::fun(s1, s2) << std::endl;
    std::cout << "LENGTH = " << with_list::fun(s1, s2) << std::endl;
    std::cout << "LENGTH = " << with_best::fun(s1, s2) << std::endl;

    return 0;
}
#endif