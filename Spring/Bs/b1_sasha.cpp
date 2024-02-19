#include <iostream>

int* append_num(int* nums, const int num, int& size) {
    size++;
    int* new_seq = new int[size];


    for(int i = 0; i < size - 1; ++i) {
        new_seq[i] = nums[i];
    }
    new_seq[size - 1] = num;


    delete[] nums;
    return new_seq;
}

void sort_seq(int * nums, const int size) {
    for(int i = 0; i < size - 1; ++i) {
        int swap_num = 0;
        for(int j = 0; j < size - i - 1; ++j) {
            if (nums[j] > nums[j + 1]) {
                swap_num = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = swap_num;
            }
        }
    }
}


int
main() {
    int* sequence = 0;
    int cnt = 0;
    int cur_num = 0;


    while(std::cin >> cur_num) {
        sequence = append_num(sequence, cur_num, cnt);
    }


    sort_seq(sequence, cnt);


    for(int i = 0; i < cnt; ++i) {
        std::cout << sequence[i] << std::endl;
    }


    delete[] sequence;
    return 0;
}