#include "editor_state.h"
#include "terminal.h"
#include "output.h"

#include <string.h>
#include <stdlib.h>

void editor_find()
{
    char *query = editor_prompt("Search: %s (ESC to cancel)");
    if (NULL == query) return;

    for (int i = 0; i < get_file_lines(); i++)
    {
        editor_line *line = get_line(i);
        char* match = strstr(line->render, query);
        if (match)
        {
            set_cursor_y(i);
            set_cursor_x(editor_row_rx_to_cx(line, match - line->render));
            set_line_offset(get_file_lines());
            break;
        }
    }

    free(query);
}
