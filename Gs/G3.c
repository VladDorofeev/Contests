#include <stdio.h>
#include <stdlib.h>

int *
get_nums_from_file (FILE *fp, int *length) {
    int *nums_arr = NULL;
    int nums_length = 0;
    int num;
    while (fscanf(fp, "%d", &num) == 1) {
        nums_length++;
        nums_arr = realloc(nums_arr, nums_length * sizeof *nums_arr);
        nums_arr[nums_length - 1] = num;
    }

    *length = nums_length;
    return nums_arr;
}


void 
swap_item (int *arr, int pos1, int pos2) {
    int *p_pos1 = arr + pos1;
    int *p_pos2 = arr + pos2;
    int temp = *p_pos1;
    *p_pos1 = *p_pos2;
    *p_pos2 = temp;
}

void 
reverse_int_arr (int *arr, int length) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < length / 2; ++i) {
        swap_item(arr, i, length - i - 1);
    }
}


void
write_arr_in_file (FILE *fp, int *arr, int length) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < length; ++i) {
        fprintf(fp, "%d", arr[i]);
        fprintf(fp, " ");
    }
}


int
main(int argc, char **argv) {
    FILE *fp = fopen(argv[1], "r");
    
    int nums_length;
    int *nums_arr = get_nums_from_file(fp, &nums_length);


    reverse_int_arr(nums_arr, nums_length);

    //Prepare to write
    fclose(fp);
    fp = fopen(argv[1], "w");

    write_arr_in_file(fp, nums_arr, nums_length);

    free(nums_arr);
    fclose(fp);
    return 0;
}