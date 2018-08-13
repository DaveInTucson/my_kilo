#include "editor_state.h"
#include "terminal.h"
#include "output.h"

#include <string.h>
#include <stdlib.h>

void editor_find_callback(char *query, int key)
{
    static int last_match = -1;
    static int direction = 1;

    if ('\r' == key  || '\x1b' == key)
    {
        last_match = -1;
        direction = 1;
        return;
    }

    if (ARROW_RIGHT == key || ARROW_DOWN == key)
    {
        direction = 1;
    }
    else if (ARROW_LEFT == key || ARROW_UP == key)
    {
        direction = -1;
    }
    else
    {
        last_match = -1;
        direction = 1;
    }

    if (last_match == -1) direction = 1;
    int current = last_match;
    for (int i = 0; i < get_file_lines(); i++)
    {
        current += direction;
        if (current == -1) 
            current = get_file_lines() -1;
        else if (current >= get_file_lines())
            current = 0;
        
        editor_line *line = get_line(current);
        char *match = strstr(line->render, query);
        if (match)
        {
            last_match = current;
            set_cursor_y(current);
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

    char *query = editor_prompt("Search: %s (Use ESC/Arrows/Enter)", 
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
