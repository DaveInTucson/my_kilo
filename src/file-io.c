#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "file-io.h"

#include "editor_state.h"

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "die.h"
#include "dlog.h"
#include "editor_state.h"
#include "terminal.h"

void editor_open(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) die ("fopen");

    set_filename(filename);
    
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) != -1)
    {
	while (linelen > 0)
	{
	    if (line[linelen-1] != '\n' && line[linelen-1] != '\r')
		break;
	    linelen--;
	}

	append_file_line(line, linelen);
    }

    free(line);
    fclose(fp);
    clear_dirty();
}

void editor_save()
{
    if (NULL == get_filename(NULL))
    {
        char *name = editor_prompt("Save as: %s (ESC to cancel)", NULL);
        if (NULL == name)
        {
            editor_set_status_message("Save aborted");
            return;
        }
        set_filename(name);
    }

    int fd = open(get_filename(NULL), O_RDWR | O_CREAT, 0644);

    if (fd != -1 && ftruncate(fd, 0) >= 0)
    {
        int len = 0;
        for (int i = 0; i < get_file_lines(); i++)
        {
            write(fd, get_line_chars(i), get_line_size(i));
            write(fd, "\n", 1);
            len += get_line_size(i) + 1;
        }
        
        close(fd);
        clear_dirty();
        editor_set_status_message("%d bytes written to disk", len);
        return;
    }

    editor_set_status_message("Can't save! I/O error: %s", strerror(errno));
    if (fd != -1) close(fd);
}
