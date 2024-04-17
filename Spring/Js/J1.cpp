#include <iostream>



bool is_in_language(std::string &s) {
    // s must be (ab)^n c^n , n >= 0
    if (s.length() == 0) {
        return true;
    }

    if (s.length() % 3 != 0) {
        return false;
    }


    std::string::size_type n_ab = 0;
    std::string::size_type n_c = 0;
    for (std::string::size_type i = 0; i < s.length() - 1; i++) {
        if (i + 1 == s.length()) {
            if (s[i] != 'c') {
                return false;
            }
        }
        if (s[i] == 'a' && s[i+1] == 'b') {
            //all good, go next pair of character
            i++;
            n_ab++;
        } else {
            n_c++;
            if (n_c >= n_ab) {
                return false;
            }
            if (s[i] == 'a' && s[i+1] != 'b') {
                return false;
            }

            if (s[i] == 'c' && s[i+1] == 'c') {
                //all good, go next pair of character
                i++;
            } else {
                return false;
            }
        }
    }
    return true;
}


int main() {
    std::cout << "0\n2\n";
    std::string str;
    while (std::getline(std::cin, str)) {
        if (is_in_language(str)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
}
