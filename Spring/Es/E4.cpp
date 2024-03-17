#include <iostream>

int
main() {
    std::cout << "D()" << std::endl;
    std::cout << "A()" << std::endl;
    std::cout << "B()" << std::endl;
    std::cout << "A(const A &)" << std::endl;
    std::cout << "E()" << std::endl;
    std::cout << "A()" << std::endl;
    std::cout << "B()" << std::endl;
    std::cout << "C()" << std::endl;
    std::cout << "~C()" << std::endl;
    std::cout << "~B()" << std::endl;
    std::cout << "~A()" << std::endl;
    std::cout << "~E()" << std::endl;
    std::cout << "~A()" << std::endl;
    std::cout << "~B()" << std::endl;
    std::cout << "~A()" << std::endl;
    std::cout << "~D()" << std::endl;
    return 0;      
}