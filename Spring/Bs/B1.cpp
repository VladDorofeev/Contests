#include <iostream>
#include <algorithm>
#include <vector>

enum { START_SZ = 3 };

int compare_ints(const void* a, const void* b)
{
    int arg1 = *static_cast<const int*>(a);
    int arg2 = *static_cast<const int*>(b);
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int
main() {
    int *arr = NULL;
    int cap = 0;
    int len = 0;
    int num;

    while (std::cin >> num) {
        if (len == cap) {
            cap = cap == 0 ? START_SZ : cap * 2;
            int *temp = new int[cap];
            for (int i = 0; i < len; i++) {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
        }
        arr[len++] = num;
    }

    //std::sort(arr, arr + len);
    if (arr != NULL) {
        qsort(arr, len, sizeof *arr, compare_ints);
    }

    for (int i = 0; i < len; i++) {
        std::cout << arr[i] << std::endl;
    }

    delete[] arr;
    return 0;
}