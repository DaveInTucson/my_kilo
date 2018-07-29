#include "terminal.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <ctype.h>

#include "editor_state.h"
#include "file-io.h"
#include "die.h"
#include "strings.h"
#include "output.h"

#define KILO_QUIT_TIMES 3

#define CTRL_KEY(k) ((k) & 0x1f)
const char ESCAPE_CHAR = '\x1b';

enum editorKey {
    BACKSPACE = 127,
    ARROW_LEFT  = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
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

    /* Special key sequences:
     * <esc> [ <digit> ~
     * <esc> [ [ABCDHF]
     * <esc> O [HF]
     */

    /* bug: these will be lost if it's a key sequence we don't recognize... */
    char c1, c2;
    if (read(STDIN_FILENO, &c1, 1) != 1) return ESCAPE_CHAR;
    if (read(STDIN_FILENO, &c2, 1) != 1) return ESCAPE_CHAR;
    if (c1 == '[')
    {
	if (isdigit(c2))
	{
	    if (read(STDIN_FILENO, &c1, 1) != 1) return ESCAPE_CHAR;
	    if (c1 != '~') return ESCAPE_CHAR;
	}
	
	switch (c2)
	{
	case '1': return HOME_KEY;
	case '3': return DEL_KEY;
	case '4': return END_KEY;
	case '5': return PAGE_UP;
	case '6': return PAGE_DOWN;
	case '7': return HOME_KEY;
	case '8': return END_KEY;
		
	case 'A': return ARROW_UP;
	case 'B': return ARROW_DOWN;
	case 'C': return ARROW_RIGHT;
	case 'D': return ARROW_LEFT;
	case 'H': return HOME_KEY;
	case 'F': return END_KEY;
	}
    }
    else if (c1 == 'O')
    {
	switch (c2)
	{
	case 'H': return HOME_KEY;
	case 'F': return END_KEY;
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


void editor_move_cursor(int dcx, int dcy)
{
    int cx = get_cursor_x() + dcx;
    int cy = get_cursor_y() + dcy;

    if (cx < 0)
    {
        if (cy > 0)
        {
            cy--;
            cx = get_line_size(cy);
        }
        else
            cx = 0;
    }
    
    if (cy < 0)
	cy = 0;
    if (cy >= get_file_lines())
        cy = get_file_lines();

    int max_cx = get_line_size(cy);
    if (cx > max_cx)
    {
        if (dcx == 1 && dcy == 0 && cy < get_file_lines())
        {
            cx = 0;
            cy++;
        }
        else
            cx = max_cx;
    }
    
    set_cursor_x(cx);
    set_cursor_y(cy);
}


char *editor_prompt(char *prompt)
{
    size_t bufsize = 128;
    char *buf = malloc(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';

    while(1)
    {
        editor_set_status_message(prompt, buf);
        editor_refresh_screen();

        int c = editor_read_key();
        if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE)
        {
            if (buflen != 0)
                buf[--buflen] = '\0';
        }
        
        if (c == ESCAPE_CHAR)
        {
            editor_set_status_message("");
            free(buf);
            return NULL;
        }
        
        if (c == '\r')
        {
            if (buflen != 0)
                editor_set_status_message("");
            return buf;
        }
        else if (!iscntrl(c) && c < 128)
        {
            if (buflen == bufsize - 1)
            {
                bufsize *= 2;
                buf = realloc(buf, bufsize);
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }
    }
}


void editor_process_keypress(keypress_t c)
{
    static int quit_times = KILO_QUIT_TIMES;

    string_const clear_screen = get_clear_screen_str();
    string_const home_cursor  = get_home_cursor_str();

    switch (c)
    {
    case '\r':
        editor_insert_newline();
        break;

    case CTRL_KEY('q'):
        if (is_dirty() && quit_times > 0)
        {
            editor_set_status_message("WARNING!!! File has unsaved changes. "
                                      "Press Ctrl-Q %d more times to quit.",
                                      quit_times);
            quit_times--;
            return;
        }
        
        write_str(clear_screen);
        write_str(home_cursor);
        exit(0);
        break;

    case CTRL_KEY('s'):
        editor_save();
        break;

    case HOME_KEY:
	set_cursor_x(0);
	break;

    case END_KEY:
        if (get_cursor_y() < get_file_lines())
            set_cursor_x(get_line_size(get_cursor_y()));
	break;
	
    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
        if (c == DEL_KEY) editor_move_cursor(-1, 0);
        editor_del_char();
        break;

    case PAGE_UP: 
        editor_move_cursor(0, -get_screen_height());
	break;
	
    case PAGE_DOWN:
        editor_move_cursor(0, get_screen_height());
	break;

    case ARROW_UP   : editor_move_cursor( 0, -1); break;
    case ARROW_DOWN : editor_move_cursor( 0,  1); break;
    case ARROW_LEFT : editor_move_cursor(-1,  0); break;
    case ARROW_RIGHT: editor_move_cursor( 1,  0); break;

    case CTRL_KEY('l'):
    case '\x1b':
        break;

    default:
        editor_insert_char(c);
    }
}
