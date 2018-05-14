#include "terminal.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <ctype.h>

#include "editor_state.h"
#include "die.h"
#include "strings.h"

#define CTRL_KEY(k) ((k) & 0x1f)
const char ESCAPE_CHAR = '\x1b';

enum editorKey {
    ARROW_LEFT  = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
};

static int write_str(string_const sc)
{
    ssize_t rc = write(STDOUT_FILENO, sc.s, sc.length);
    if (rc == -1 || rc != sc.length) return -1;
    return sc.length;
 }

keypress_t editor_read_key()
{
    int nread;
    char c;

    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    if (c != ESCAPE_CHAR) return c;

    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1) return ESCAPE_CHAR;
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return ESCAPE_CHAR;
    
    if (seq[0] == '[')
    {
	if (isdigit(seq[1]))
	{
	    if (read(STDIN_FILENO, &seq[2], 1) != 1) return ESCAPE_CHAR;
	    if (seq[2] == '~')
	    {
		switch (seq[1])
		{
		case '5': return PAGE_UP;
		case '6': return PAGE_DOWN;
		}
	    }
	}

	switch (seq[1])
	{
	case 'A': return ARROW_UP;
	case 'B': return ARROW_DOWN;
	case 'C': return ARROW_RIGHT;
	case 'D': return ARROW_LEFT;
	}
    }

    return ESCAPE_CHAR;
}

int get_cursor_position(int *rows, int *cols)
{
    if (write_str(get_read_cursor_pos_str()) == -1)
        return -1;
    
    char buf[32];
    unsigned int i = 0;
    for (i = 0; i < sizeof(buf); i++)
    {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
    }
    
    buf[i] = '\0';
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
        return -1;

    return 0;

}

int get_window_size(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1 && ws.ws_col != 0)
    {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }

    if (write_str(get_antihome_cursor_str()) != -1)
    {
        return get_cursor_position(rows, cols);
    }

    *rows = *cols = 0;
    return -1;
}


void editor_set_cursor_row(int cx)
{
    g_editor_state.cy = cx;
}

void editor_move_cursor(int dcx, int dcy)
{
    g_editor_state.cx += dcx;
    g_editor_state.cy += dcy;

    if (g_editor_state.cx < 0)
	g_editor_state.cx = 0;
    if (g_editor_state.cx >= g_editor_state.screencols)
	g_editor_state.cx = g_editor_state.screencols - 1;
    
    if (g_editor_state.cy < 0)
	g_editor_state.cy = 0;
    if (g_editor_state.cy >= g_editor_state.screenrows)
	g_editor_state.cy = g_editor_state.screenrows - 1;
}


void editor_process_keypress(keypress_t c)
{
    string_const clear_screen = get_clear_screen_str();
    string_const home_cursor  = get_home_cursor_str();

    switch (c)
    {
    case CTRL_KEY('q'):
        write_str(clear_screen);
        write_str(home_cursor);
        exit(0);
        break;

    case PAGE_UP: 
	editor_set_cursor_row(0);
	break;
	
    case PAGE_DOWN:
	editor_set_cursor_row(g_editor_state.screenrows-1);
	break;

    case ARROW_UP   : editor_move_cursor( 0, -1); break;
    case ARROW_DOWN : editor_move_cursor( 0,  1); break;
    case ARROW_LEFT : editor_move_cursor(-1,  0); break;
    case ARROW_RIGHT: editor_move_cursor( 1,  0); break;
    }
}
