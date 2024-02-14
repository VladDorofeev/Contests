#include <stdio.h>

enum {COUNT_BYTE = 1, INVERT2 = 1, INVERT4 = 3};

int
main(int argc, char **argv)
{
    FILE* byte_file = fopen(argv[1], "r+b");
    char byte = 0;

    if (fread(&byte, sizeof(byte), COUNT_BYTE, byte_file) == 1) {
        fseek(byte_file, 0, SEEK_SET);
        byte ^= (1<<INVERT2);
        byte ^= (1<<INVERT4);
        fwrite(&byte, sizeof(byte), COUNT_BYTE, byte_file);
    }
    fclose(byte_file);
    return 0;
}
