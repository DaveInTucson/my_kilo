#include "dlog.h"

#include <stdio.h>
#include <stdarg.h>

void dlog(const char *format, ...)
{
    FILE* f = fopen("Dlog.txt", "a");
    if (f)
    {
        va_list args;
        va_start(args, format);
        vfprintf(f, format, args);

        fclose(f);
    }
}
