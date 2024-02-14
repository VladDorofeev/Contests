#include <stdio.h>

enum { SYSTEM_SIZE = 1 << 20, BLOCKS_SIZE = 13 };

typedef struct {
    long blocks[BLOCKS_SIZE];
    unsigned int file_size;
} Inode;


typedef struct {
    Inode item[SYSTEM_SIZE];
    long root;
} File_System;


File_System file_system;

int
main (void) {
    file_system.item[10].blocks[0] = 1038;
    file_system.item[10].blocks[1] = 37465;
    return 0;
}