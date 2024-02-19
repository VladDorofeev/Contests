#include <iostream>

class Sequence {
    private:
        int* massive;
        int size;
    public:
        Sequence();
        ~Sequence();
        void append_num(const int);
        void sort_seq();
        int get_num(const int);
        int get_size();
};

Sequence::Sequence() {
    this->massive = 0;
    this->size = 0;
}
Sequence::~Sequence() {
    delete[] this->massive;
}

void Sequence::append_num(const int num) {
    this->size++;
    int* new_seq = new int[this->size];


    for(int i = 0; i < this->size - 1; ++i) {
        new_seq[i] = this->massive[i];
    }
    new_seq[this->size - 1] = num;


    delete[] this->massive;
    this->massive = new_seq;
}

void Sequence::sort_seq() {
    for(int i = 0; i < this->size - 1; ++i) {
        int swap_num = 0;
        for(int j = 0; j < this->size - i - 1; ++j) {
            if (this->massive[j] > this->massive[j + 1]) {
                swap_num = this->massive[j];
                this->massive[j] = this->massive[j + 1];
                this->massive[j + 1] = swap_num;
            }
        }
    }
}

int Sequence::get_num(const int pos) {
    return this->massive[pos];
}

int Sequence::get_size() {
    return this->size;
}


int
main() {
    Sequence seq;
    int cur_num;

    while(std::cin >> cur_num) {
        seq.append_num(cur_num);
    }


    seq.sort_seq();


    int size = seq.get_size();
    for(int i = 0; i < size; ++i) {
        std::cout << seq.get_num(i) << std::endl;
    }

    return 0;
}
