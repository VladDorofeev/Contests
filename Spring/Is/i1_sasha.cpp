#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

template<typename T>
int fun_vec_list_set(std::string const &s1, std::string const &s2) {
        if (s1.length() == 0 && s2.length() == 0) {
            return 1;
        }
        T obj1;
        for(size_t i = 0; i < s1.length(); ++i) {
            for(size_t j = 0; j <= s1.length() - i; ++j) {
                if (std::find(obj1.begin(), obj1.end(), s1.substr(i, j)) == obj1.end()) {
                    obj1.push_back(s1.substr(i, j));
                }
            }
        }
        T obj2;
        for(size_t i = 0; i < s2.length(); ++i) {
            for(size_t j = 0; j <= s2.length() - i; ++j) {
                if (std::find(obj2.begin(), obj2.end(), s2.substr(i, j)) == obj2.end()) {
                    obj2.push_back(s2.substr(i, j));
                }
            }
        }
        if (obj1.size() == 0) {
            return obj2.size();
        } else if (obj2.size() == 0) {
            return obj1.size();
        }
        T obj3;
        for(typename T::iterator it1 = obj1.begin(); it1 != obj1.end(); ++it1) {
            for(typename T::iterator it2 = obj2.begin(); it2 != obj2.end(); ++it2) {
                if (std::find(obj3.begin(), obj3.end(), *it1 + *it2) == obj3.end()) {
                    obj3.push_back(*it1 + *it2);
                }
                if (std::find(obj3.begin(), obj3.end(), *it2 + *it1) == obj3.end()) {
                    obj3.push_back(*it2 + *it1);
                }
            }
        }
        return obj3.size();
}
template<>
int fun_vec_list_set<std::set<std::string> >(std::string const &s1, std::string const &s2) {
        if (s1.length() == 0 && s2.length() == 0) {
            return 1;
        }
        std::set<std::string> obj1;
        for(size_t i = 0; i < s1.length(); ++i) {
            for(size_t j = 0; j <= s1.length() - i; ++j) {
                obj1.insert(s1.substr(i, j));
            }
        }
        std::set<std::string> obj2;
        for(size_t i = 0; i < s2.length(); ++i) {
            for(size_t j = 0; j <= s2.length() - i; ++j) {
                obj2.insert(s2.substr(i, j));
            }
        }
        if (obj1.size() == 0) {
            return obj2.size();
        } else if (obj2.size() == 0) {
            return obj1.size();
        }
        std::set<std::string> obj3;
        for(std::set<std::string>::iterator it1 = obj1.begin(); it1 != obj1.end(); ++it1) {
            for(std::set<std::string>::iterator it2 = obj2.begin(); it2 != obj2.end(); ++it2) {
                obj3.insert(*it1 + *it2);
                obj3.insert(*it2 + *it1);
            }
        }
        return obj3.size();
}


namespace with_vector {
    int fun(std::string const &s1, std::string const &s2) {
        return fun_vec_list_set<std::vector<std::string> >(s1, s2);
    }
}


namespace with_list {
    int fun(std::string const &s1, std::string const &s2) {
        return fun_vec_list_set<std::list<std::string> >(s1, s2);
    }
}

namespace with_best {
    int fun(std::string const &s1, std::string const &s2) {
        return fun_vec_list_set<std::set<std::string> >(s1, s2);
    }
}

/*int main(){}*/
