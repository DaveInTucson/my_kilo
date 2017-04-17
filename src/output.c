#include "output.h"
#include "strings.h"
#include "editor_state.h"
#include "term_buffer.h"

#include <stdio.h>

const char* g_kilo_version = "0.0.1";

void append_welcome_message(term_buffer* tb)
{
    char buffer[80];

    int buffer_len = snprintf(buffer, sizeof(buffer),
                              "Kilo editor -- version %s", g_kilo_version);
    if (buffer_len > g_editor_state.screencols)
        buffer_len = g_editor_state.screencols;

    int padding = (g_editor_state.screencols - buffer_len) / 2;
    if (padding)
    {
        tb_append_str(tb, get_tilde_str());
        padding--;
    }

    while (padding--) tb_append_str(tb, get_space_str());
    
    tb_append(tb, buffer, buffer_len);
}


void editorDrawRows(term_buffer* tb)
{
    string_const tilde = get_tilde_str();
    string_const rn    = get_rn_str();
    
    int y;
    for (y = 0; y < g_editor_state.screenrows; y++)
    {
        if (y != g_editor_state.screenrows/3)
            tb_append_str(tb, tilde);
        else
            append_welcome_message(tb);
        
        tb_append_str(tb, get_clear_row_str());
        if (y + 1 < g_editor_state.screenrows)
            tb_append_str(tb, rn);
    }
}


void editorRefreshScreen()
{
    term_buffer tb;

    tb_init(&tb);
    
    string_const home_cursor = get_home_cursor_str();

    tb_append_str(&tb, get_cursor_off_str());
    tb_append_str(&tb, home_cursor);

    editorDrawRows(&tb);

    tb_append_str(&tb, home_cursor);
    tb_append_str(&tb, get_cursor_on_str());
    
    tb_write(&tb);
    tb_free(&tb);
}

