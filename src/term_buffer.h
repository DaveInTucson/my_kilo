#ifndef TERM_BUFFER_H
#define TERM_BUFFER_H

#include "strings.h"

typedef struct
{
    char buffer[1024*64];
    int length;
} term_buffer;

void tb_init(term_buffer* tb);
void tb_append(term_buffer* tb, const char* buffer, unsigned int length);
void tb_write(term_buffer* tb);
void tb_free(term_buffer* tb);

static inline void tb_append_str(term_buffer* tb, string_const sc)
{ tb_append(tb, sc.s, sc.length); }

#endif /* TERM_BUFFER_H */
