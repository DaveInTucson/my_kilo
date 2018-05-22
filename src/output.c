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
    if (buffer_len > get_screen_width())
        buffer_len = get_screen_width();

    int padding = (get_screen_width() - buffer_len) / 2;
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
    for (int y = 0; y < get_screen_height(); y++)
    {
	if (y < get_file_lines())
	{
	    int len = get_line_size(y);
	    if (len > get_screen_width())
		len = get_screen_width();
	    tb_append(tb, get_line_chars(y), len);
	}
        else if (y != get_screen_height()/3)
            tb_append_str(tb, get_tilde_str());
        else if (get_file_lines() == 0)
            append_welcome_message(tb);
        
        tb_append_str(tb, get_clear_row_str());
        if (y + 1 < get_screen_height())
            tb_append_str(tb, get_rn_str());
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

    editor_position_cursor(&tb, get_cursor_x(), get_cursor_y());

    tb_append_str(&tb, get_cursor_on_str());
    
    tb_write(&tb);
    tb_free(&tb);
}

