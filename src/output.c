#include "output.h"
#include "strings.h"
#include "editor_state.h"
#include "term_buffer.h"
#include "dlog.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        int line_row = y + get_line_offset();
	
	if (line_row < get_file_lines())
	{
	    int len = get_render_size(line_row) - get_col_offset();
            if (len < 0) len = 0;
	    if (len > get_screen_width())
		len = get_screen_width();
	    tb_append(tb, get_render_chars(line_row) + get_col_offset(), len);
	}
        else if (get_file_lines() == 0 && y == get_screen_height()/3)
            append_welcome_message(tb);
        else
        {
            tb_append_str(tb, get_tilde_str());
        }
        
        tb_append_str(tb, get_clear_row_str());
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

int editor_row_cx_to_rx(editor_line *line, int cx)
{
    int rx = 0;

    for (int j = 0; j < cx; j++)
    {
        if (line->chars[j] == '\t')
            rx += (KILO_TAB_STOP - 1) - (rx % KILO_TAB_STOP);
        rx++;
    }

    return rx;
}


void editor_scroll()
{
    set_cursor_rx(0);
    if (get_cursor_y() < get_file_lines())
    {
        set_cursor_rx(editor_row_cx_to_rx(
                          get_line(get_cursor_y()),
                          get_cursor_x()));
    }

    
    if (get_cursor_y() < get_line_offset())
    {
        set_line_offset(get_cursor_y());
    }

    if (get_cursor_y() >= get_line_offset() + get_screen_height())
    {
        set_line_offset(get_cursor_y() - get_screen_height() + 1);
    }

    if (get_cursor_rx() < get_col_offset())
    {
        set_col_offset(get_cursor_rx());
    }

    if (get_cursor_rx() >= get_col_offset() + get_screen_width())
    {
        set_col_offset(get_cursor_rx() - get_screen_width() + 1);
    }
}

void editor_draw_status_bar(term_buffer* tb)
{
    tb_append_str(tb, get_colors_invert_str());

    char status[80], rstatus[80];
    int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
                       get_filename("[No name]"),
                       get_file_lines(),
                       is_dirty() ? "(modified)" : "");
    
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d",
                        get_cursor_y() + 1, get_file_lines());
    
    if (len > get_screen_width())
        len = get_screen_width();
    tb_append(tb, status, len);
    
    while (len < get_screen_width())
    {
        if (get_screen_width() - len == rlen)
        {
            tb_append(tb, rstatus, rlen);
            break;
        }
        tb_append(tb, " ", 1);
        len++;
    }
    
    tb_append_str(tb, get_colors_normal_str());
    tb_append_str(tb, get_rn_str());
}

void editor_draw_message_bar(term_buffer* tb)
{
    tb_append_str(tb, get_clear_row_str());

    int msglen = strlen(get_status_buffer());
    if (msglen > get_screen_width())
        msglen = get_screen_width();
    if (msglen && time(NULL) - get_status_time() < 5)
        tb_append(tb, get_status_buffer(), msglen);
}




void editor_refresh_screen()
{
    editor_scroll();

    term_buffer tb;

    tb_init(&tb);
    
    string_const home_cursor = get_home_cursor_str();

    tb_append_str(&tb, get_cursor_off_str());
    tb_append_str(&tb, home_cursor);

    editor_draw_rows(&tb);
    editor_draw_status_bar(&tb);
    editor_draw_message_bar(&tb);
    
    editor_position_cursor(&tb, 
                           get_cursor_rx() - get_col_offset(),
                           get_cursor_y() - get_line_offset());

    tb_append_str(&tb, get_cursor_on_str());
    
    tb_write(&tb);
    tb_free(&tb);
}


/* last line */
