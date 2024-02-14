#include <stdio.h>

enum {BUF_SIZE = 2 << 19, COUNT_BLOCKS = 13};

struct inode
{
    unsigned size_file;
    long array_blocks[COUNT_BLOCKS];
};


struct file_sys
{
    struct inode arr[BUF_SIZE];
    int num_block;
};

struct file_sys my_file_sys;


int
main(void){
    my_file_sys.arr[10].array_blocks[0] = 1038;
    my_file_sys.arr[10].array_blocks[1] = 37465;
    return 0;
}