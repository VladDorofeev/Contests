#include <iostream>


class Seq
{
public:
    Seq(int _sz);
    ~Seq();
    int &item(int pos);
    friend void print_seq(Seq &sq);
private:
    int *arr;
    int sz;
};

Seq::Seq(int _sz)
{
    arr = new int[_sz];
    this->sz = _sz;
}

Seq::~Seq()
{
    delete[] arr;
}

int &
Seq::item(int pos) {
    if (pos >= sz) {
        std::cout << "Out of bound" << std::endl;
        delete[] this->arr;
        exit(0);
    }
    return this->arr[pos];
}

void
print_seq(Seq &sq) {
    for (int i = 0; i < sq.sz - 1; i++) {
        std::cout << sq.arr[i] << ' ';
    }
    std::cout << sq.arr[sq.sz - 1] << std::endl;
}

enum { SZ = 10 };

int
main() {
    Seq sq(SZ);
    int num = 0;
    for (int i = 0; i < SZ; i++ ){
        std::cin >> num;
        sq.item(i) = num;
    }
    print_seq(sq);
    return 0;
}