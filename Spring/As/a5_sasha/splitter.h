#ifndef SPLITTER_H
#define SPLITTER_H

void set_array(int, const char**);

void set_delimiter(const char*);

int next_block(int*, int*);

void free_splitter(void);

#endif
