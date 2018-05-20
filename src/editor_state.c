#include "editor_state.h"
#include "die.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

editor_state g_editor_state;

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

void add_file_line(char* line, ssize_t linelen)
{
    g_editor_state.line.size = linelen;
    g_editor_state.line.chars = malloc(linelen + 1);
    memcpy(g_editor_state.line.chars, line, linelen);
    g_editor_state.line.chars[linelen] = '\0';
    g_editor_state.numlines = 1;
}

