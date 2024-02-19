#include <iostream>


class A {
    private:
        int x;
    public:
        void m() {
            A* a = new A;
            delete a;
            a = new A(4.20);
            A* b = new A(*a); 
            delete a;
            delete b;
            b = new A(*this);
            a = new A(2.32, 2);
            delete a;
            delete b;
        }
        A() {
            this->x = 0;
            std::cout << "1\n";
        }
        A(const A &other) {
            this->x = other.x;
            std::cout << "2\n";
        }
        A(double num){
            this->x = static_cast<int>(num);
            std::cout << "3\n";
        }
        A(float num1, unsigned short num2) {
            this->x = static_cast<int>(num1);
            std::cout << "4\n";
        }
        ~A() {
            std::cout << "5\n";
        }
};