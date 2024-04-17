#include <iostream>

// S → aBA | ε
// B → bSA
// AA → c
bool scan_g(std::string & str) {
    std::string::size_type count1 = 0;
    std::string::size_type count2 = 0;
    bool read_a = false;
    bool read_b = false;
    bool read_c = false;
    std::string::size_type str_len = str.length();
    for(std::string::size_type i = 0; i < str_len; ++i) {
        switch (str[i]) {
        case 'a':
            if (read_a || read_c) {
                return false;
            }
            read_a = true;
            read_b = false;
            break;
        case 'b':
            if (!read_a || read_b || read_c) {
                return false;
            }
            read_b = true;
            read_a = false;
            ++count1;
            break;
        case 'c':
            if (read_a) {
                return false;
            }
            read_c = true;
            ++count2;
            break;
        default:
            return false;
            break;
        }
    }
    if (str_len != 0 && (count1 == 0 || count2 == 0)) {
        return false;
    }
    return count1 == count2;
}

int
main()
{
    std::cout << "0\n2\n";
    std::string str;
    while (std::getline(std::cin, str)) {
        if (scan_g(str)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
}
