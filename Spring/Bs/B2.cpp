#include <iostream>
#include <algorithm>
#include <vector>

enum { START_SZ = 3 };

class Seq 
{
    int *arr;
    int cap;
    int len;
public:
    Seq();
    ~Seq();
    void add_num(int num);
    void sort();

    int get_len();
    int get_num(int i);
};

int
main() {
    Seq seq;
    int num;

    while (std::cin >> num) {
        seq.add_num(num);
    }

    seq.sort();

    for (int i = 0; i < seq.get_len(); i++) {
        std::cout << seq.get_num(i) << std::endl;
    }

    return 0;
}





Seq::Seq() {
    this->arr = NULL;
    this->cap = 0;
    this->len = 0;
}

void
Seq::add_num(int num) {
    if (this->len == this->cap) {
        this->cap = this->cap == 0 ? START_SZ : this->cap * 2;
        int *temp = new int[this->cap];
        for (int i = 0; i < this->len; i++) {
            temp[i] = this->arr[i];
        }
        delete[] this->arr;
        this->arr = temp;

        
    }
    arr[this->len++] = num;
}

void
Seq::sort() {
    std::sort(this->arr, this->arr + this->len);
}
Seq::~Seq() {
    delete[] this->arr;
}

int Seq::get_len() {
    return this->len;
}

int Seq::get_num(int i) {
    return this->arr[i];
}
