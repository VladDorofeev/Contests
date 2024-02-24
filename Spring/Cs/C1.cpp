#include <iostream>

class Seq
{
public:
    Seq(int _sz);
    Seq(Seq &sq);
    Seq& operator =(const Seq &sq);
    ~Seq();

    int &item(int pos) const;
    friend void print_seq(const Seq &sq);
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

Seq::Seq(Seq &sq) {
    arr = new int[sq.sz];
    for (int i = 0; i < sq.sz; i++) {
        arr[i] = sq.arr[i];
    }
    sz = sq.sz;
}

Seq& 
Seq::operator =(const Seq &sq) {
    arr = new int[sq.sz];
    for (int i = 0; i < sq.sz; i++) {
        arr[i] = sq.arr[i];
    }
    sz = sq.sz;
    return *this;
}

int &
Seq::item(int pos) const {
    if (pos >= sz) {
        std::cout << "Out of bound" << std::endl;
        delete[] this->arr;
        exit(0);
    }
    return this->arr[pos];
}

void
print_seq(const Seq &sq) {
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