#include "terminal.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "editor_state.h"
#include "die.h"
#include "strings.h"

#define CTRL_KEY(k) ((k) & 0x1f)

static int write_str(string_const sc)
{
    ssize_t rc = write(STDOUT_FILENO, sc.s, sc.length);
    if (rc == -1 || rc != sc.length) return -1;
    return sc.length;
 }

char editor_read_key()
{
    int nread;
    char c;

    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    return c;
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
    g_editor_state.cx += dcx;
    g_editor_state.cy += dcy;
}


void editor_process_keypress(char c)
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

    case 'w': editor_move_cursor( 0, -1); break;
    case 's': editor_move_cursor( 0,  1); break;
    case 'a': editor_move_cursor(-1,  0); break;
    case 'd': editor_move_cursor( 1,  0); break;
    }
}
