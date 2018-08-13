#include "editor_state.h"
#include "terminal.h"
#include "output.h"

#include <string.h>
#include <stdlib.h>

void editor_find_callback(char *query, int key)
{
    if ('\r' == key  || '\x1b' == key)
        return;

    for (int i = 0; i < get_file_lines(); i++)
    {
        editor_line *line = get_line(i);
        char *match = strstr(line->render, query);
        if (match)
        {
            set_cursor_y(i);
            set_cursor_x(editor_row_rx_to_cx(line, match - line->render));
            set_line_offset(get_file_lines());
            break;
        }
    }
}


void editor_find()
{
    int saved_cx = get_cursor_x();
    int saved_cy = get_cursor_y();
    int saved_col_offset = get_col_offset();
    int saved_line_offset = get_line_offset();

    char *query = editor_prompt("Search: %s (ESC to cancel)", 
                                editor_find_callback);

    if (query)
        free(query);
    else
    {
        set_cursor_x(saved_cx);
        set_cursor_y(saved_cy);
        set_col_offset(saved_col_offset);
        set_line_offset(saved_line_offset);
    }
}
