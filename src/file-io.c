#include "file-io.h"

#include "editor_state.h"

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

void editor_open()
{
    char *line = "Hello, world!";
    ssize_t linelen = 13;

    g_editor_state.row.size = linelen;
    g_editor_state.row.chars = malloc(linelen + 1);
    memcpy(g_editor_state.row.chars, line, linelen);
    g_editor_state.row.chars[linelen] = '\0';
    g_editor_state.numrows = 1;
}
