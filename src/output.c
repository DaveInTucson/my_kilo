#include "output.h"
#include "strings.h"
#include "editor_state.h"
#include "term_buffer.h"

#include <stdio.h>
#include <string.h>

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


void editor_draw_rows(term_buffer* tb)
{
    string_const tilde = get_tilde_str();
    string_const rn    = get_rn_str();
    
    int y;
    for (y = 0; y < g_editor_state.screenrows; y++)
    {
	if (y < g_editor_state.numrows)
	{
	    int len = g_editor_state.row.size;
	    if (len > g_editor_state.screencols)
		len = g_editor_state.screencols;
	    tb_append(tb, g_editor_state.row.chars, len);
	}
        else if (y != g_editor_state.screenrows/3)
            tb_append_str(tb, tilde);
        else
            append_welcome_message(tb);
        
        tb_append_str(tb, get_clear_row_str());
        if (y + 1 < g_editor_state.screenrows)
            tb_append_str(tb, rn);
    }
}


void editor_position_cursor(term_buffer *tb, int cx, int cy)
{
    char buf[32];

    string_const cpf = get_cursor_pos_fmt_str();
    snprintf(buf, sizeof(buf), cpf.s, cy+1, cx+1);
    tb_append(tb, buf, strlen(buf));
}

void editor_refresh_screen()
{
    term_buffer tb;

    tb_init(&tb);
    
    string_const home_cursor = get_home_cursor_str();

    tb_append_str(&tb, get_cursor_off_str());
    tb_append_str(&tb, home_cursor);

    editor_draw_rows(&tb);

    editor_position_cursor(&tb, g_editor_state.cx, g_editor_state.cy);

    tb_append_str(&tb, get_cursor_on_str());
    
    tb_write(&tb);
    tb_free(&tb);
}

