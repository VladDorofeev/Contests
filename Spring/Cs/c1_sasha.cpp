#include <iostream>

enum {BUFSIZE = 10};

class Massive {
private:
    int size;
    int* massive;
public:
    Massive(const int &);
    Massive(const Massive&);
    ~Massive();
    int& item(const int&);
    int item(const int&) const;
    int get_size() const;
    Massive& operator =(const Massive&);
};

Massive::Massive(const int & s) {
    this->size = s;
    this->massive = new int [this->size];
}

Massive::Massive(const Massive& other_mas) {
    size = other_mas.size;
    massive = new int [size];
    for(int i = 0; i < size; ++i) {
        massive[i] = other_mas.massive[i];
    }
}

Massive::~Massive() {
    delete[] massive;
}

int& Massive::item(const int & i){
    if (i > size - 1) {
        std::cout << "Out of bound\n";
        delete[] massive;
        exit(0);
    }
    return massive[i];
}

int Massive::item(const int & i) const {
    if (i > size - 1) {
        std::cout << "Out of bound\n";
        delete[] massive;
        exit(0);
    }
    return massive[i];
}

int Massive::get_size() const {
    return size;
}

Massive& Massive::operator =(const Massive& other_mas) {
    delete[] massive;
    size = other_mas.size;
    massive = new int [size];
    for(int i = 0; i < size; ++i) {
        massive[i] = other_mas.massive[i];
    }
    return *this;
}

void print_mas(const Massive & arr) {
    int cur_size = arr.get_size();
    for(int i = 0; i < cur_size - 1; ++i) {
        std::cout << arr.item(i) << ' ';
    }
    std::cout << arr.item(cur_size - 1) << std::endl;
}

int
main() {
    Massive mas(BUFSIZE);
    for(int i = 0; i < BUFSIZE; ++i) {
        int cur_num;
        std::cin >> cur_num;
        mas.item(i) = cur_num;
    }
    print_mas(mas);
    return 0;
}