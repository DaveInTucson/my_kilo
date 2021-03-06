#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "editor_state.h"
#include "die.h"
#include "dlog.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

editor_state g_editor_state;
static const int g_tab_stop = KILO_TAB_STOP;

static inline void checked_tcgetattr(int fd, struct termios *termios_p)
{ if (tcgetattr(fd, termios_p) == -1) die("tcgetattr"); }

static inline void checked_tcsetattr(
    int fd, 
    int optional_actions,
    const struct termios *termios_p)
{ if (tcsetattr(fd, optional_actions, termios_p) == -1) die("tcsetattr"); }


void disable_raw_mode()
{
    checked_tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_editor_state.orig_termios);
}


void enable_raw_mode()
{
    checked_tcgetattr(STDIN_FILENO, &g_editor_state.orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = g_editor_state.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    checked_tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void editor_update_line(editor_line* line)
{
    int tab_count = 0;
    for (int j = 0; j < line->size; j++)
        if (line->chars[j] == '\t') tab_count++;

    free(line->render);
    line->render = malloc(line->size + tab_count*(g_tab_stop-1) + 1);
    
    int idx = 0;
    for (int j = 0; j < line->size; j++)
    {
        if (line->chars[j] == '\t')
        {
            do
                line->render[idx++] = ' ';
            while (idx % g_tab_stop != 0);
        }
        else
        {
            line->render[idx++] = line->chars[j];
        }
    }
    line->render[idx] = '\0';
    line->rsize = idx;
}


void insert_file_line(int at, char* line, ssize_t linelen)
{
    if (at < 0 || at > get_file_lines()) return;

    g_editor_state.lines = realloc(
	g_editor_state.lines,
	sizeof(editor_line) * (g_editor_state.numlines + 1));
    memmove(&g_editor_state.lines[at+1],
            &g_editor_state.lines[at],
            sizeof(editor_line) * (get_file_lines() - at));

    g_editor_state.lines[at].size = linelen;
    g_editor_state.lines[at].chars = malloc(linelen + 1);
    memcpy(g_editor_state.lines[at].chars, line, linelen);
    g_editor_state.lines[at].chars[linelen] = '\0';

    g_editor_state.lines[at].rsize = 0;
    g_editor_state.lines[at].render = NULL;
    editor_update_line(&g_editor_state.lines[at]);
    
    g_editor_state.numlines++;
    set_dirty();
}


void init_file()
{
    g_editor_state.line_offset = 0;
    g_editor_state.col_offset = 0;
    g_editor_state.numlines = 0;
    g_editor_state.lines = NULL;
    g_editor_state.filename = NULL;
    g_editor_state.dirty = 0;
    g_editor_state.statusmsg[0] = '\0';
    g_editor_state.statusmsg_time = 0;
}

void set_filename(const char* filename)
{
    free(g_editor_state.filename);
    g_editor_state.filename = strdup(filename);
}

void editor_set_status_message(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_editor_state.statusmsg, 
              sizeof(g_editor_state.statusmsg),
              fmt,
              ap);
    va_end(ap);

    g_editor_state.statusmsg_time = time(NULL);
}

void editor_line_insert_char(editor_line *line, int at, int c)
{
    if (at < 0 || at > line->size) at = line->size;
    line->chars = realloc(line->chars, line->size + 2);
    memmove(&line->chars[at + 1], &line->chars[at], line->size - at + 1);
    line->size++;
    line->chars[at] = c;
    editor_update_line(line);
    set_dirty();
}

void editor_insert_char(int c)
{
    if (get_cursor_y() == get_file_lines())
        append_file_line("", 0);

    editor_line_insert_char(get_line(get_cursor_y()), get_cursor_x(), c);
    set_cursor_x(get_cursor_x() + 1);
}

void editor_insert_newline()
{
    if (get_cursor_x() == 0)
    {
        insert_file_line(get_cursor_y(), "", 0);
    }
    else
    {
        editor_line* line = get_line(get_cursor_y());
        insert_file_line(get_cursor_y() + 1,
                           &line->chars[get_cursor_x()],
                           line->size - get_cursor_x());
        line = get_line(get_cursor_y());
        line->size = get_cursor_x();
        line->chars[line->size] = '\0';
        editor_update_line(line);
    }

    set_cursor_y(get_cursor_y() + 1);
    set_cursor_x(0);
}

void editor_line_del_char(editor_line *line, int at)
{
    if (at < 0 || at >= line->size) return;
    memmove(&line->chars[at], &line->chars[at+1], line->size - at);
    line->size--;
    editor_update_line(line);
    set_dirty();
}

void editor_free_line(editor_line *line)
{
    free(line->render);
    free(line->chars);
}

void editor_del_line(int at)
{
    if (at < 0 || at > get_file_lines()) return;
    editor_free_line(get_line(at));
    memmove(&g_editor_state.lines[at],
            &g_editor_state.lines[at+1],
            sizeof(editor_line) * (get_file_lines() - at - 1));
    g_editor_state.numlines--;
    set_dirty();
}

void editor_line_append_string(editor_line* line, char *s, size_t len)
{
    line->chars = realloc(line->chars, line->size + len + 1);
    memcpy(&line->chars[line->size], s, len);
    line->size += len;
    line->chars[line->size] = '\0';
    editor_update_line(line);
    set_dirty();
}

void editor_del_char()
{
    if (get_cursor_y() == get_file_lines()) return;
    if (get_cursor_x() == 0 && get_cursor_y() == 0) return;

    editor_line *line = get_line(get_cursor_y());
    if( get_cursor_x() > 0)
    {
        editor_line_del_char(line, get_cursor_x() - 1);
        set_cursor_x(get_cursor_x() - 1);
    }
    else
    {
        editor_line *prev_line = get_line(get_cursor_y() -1);
        set_cursor_x(prev_line->size);
        editor_line_append_string(prev_line, line->chars, line->size);
        editor_del_line(get_cursor_y());
        set_cursor_y(get_cursor_y() - 1);
    }
}
