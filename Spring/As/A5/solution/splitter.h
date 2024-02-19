#ifndef __SPLITTER_H__
#define __SPLITTER_H__

void set_array(int argc, const char *argv[]);
void set_delimiter(const char s[]); 
int next_block(int *begin, int *end);
void free_splitter(void);

#endif
 