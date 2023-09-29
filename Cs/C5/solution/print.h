#ifndef PRINT_H
#define PRINT_H

enum { BUF_SIZE = 8 };

void print(const char format[], ...);
void flush(void);
#endif /*PRINT_H*/