#include "term_buffer.h"
#include "die.h"
#include "dlog.h"

#include <string.h>
#include <unistd.h>

void tb_init(term_buffer* tb)
{ tb->length = 0; }

void tb_append(term_buffer* tb, const char* buffer, unsigned int length)
{
    if (tb->length + length > sizeof(tb->buffer))
    {
        dlog("tb length=%d, length=%d\n", tb->length, length);
        die("term_buffer overflow");
    }
    
    memcpy(&tb->buffer[tb->length], buffer, length);
    tb->length += length;
}

void tb_write(term_buffer* tb)
{
    ssize_t rc = write(STDOUT_FILENO, tb->buffer, tb->length);
    if (rc != tb->length) die ("tb_write");
}

void tb_free(term_buffer* tb)
{
    tb->length = 0;
}

