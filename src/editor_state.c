#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "editor_state.h"
#include "die.h"
#include "dlog.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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


void append_file_line(char* line, ssize_t linelen)
{
    g_editor_state.lines = realloc(
	g_editor_state.lines,
	sizeof(editor_line) * (g_editor_state.numlines + 1));

    int last = g_editor_state.numlines;
    g_editor_state.lines[last].size = linelen;
    g_editor_state.lines[last].chars = malloc(linelen + 1);
    memcpy(g_editor_state.lines[last].chars, line, linelen);
    g_editor_state.lines[last].chars[linelen] = '\0';

    g_editor_state.lines[last].rsize = 0;
    g_editor_state.lines[last].render = NULL;
    editor_update_line(&g_editor_state.lines[last]);
    
    g_editor_state.numlines++;
}

void init_file()
{
    g_editor_state.line_offset = 0;
    g_editor_state.col_offset = 0;
    g_editor_state.numlines = 0;
    g_editor_state.lines = NULL;
    g_editor_state.filename = NULL;
}

void set_filename(const char* filename)
{
    free(g_editor_state.filename);
    g_editor_state.filename = strdup(filename);
}
