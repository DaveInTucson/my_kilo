#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "file-io.h"

#include "editor_state.h"

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#include "die.h"
#include "editor_state.h"

void editor_open(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) die ("fopen");

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    linelen = getline(&line, &linecap, fp);
    if (linelen != -1)
    {
	while (linelen > 0)
	{
	    if (line[linelen-1] != '\n' && line[linelen-1] != '\r')
		break;
	    linelen--;
	}

	add_file_line(line, linelen);
    }

    free(line);
    fclose(fp);
}
