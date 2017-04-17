#include "editor_state.h"
#include "die.h"

#include <unistd.h>
#include <stdlib.h>

editorState g_editor_state;

static inline void checked_tcgetattr(int fd, struct termios *termios_p)
{ if (tcgetattr(fd, termios_p) == -1) die("tcgetattr"); }

static inline void checked_tcsetattr(
    int fd, 
    int optional_actions,
    const struct termios *termios_p)
{ if (tcsetattr(fd, optional_actions, termios_p) == -1) die("tcsetattr"); }


void disableRawMode()
{
    checked_tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_editor_state.orig_termios);
}


void enableRawMode()
{
    checked_tcgetattr(STDIN_FILENO, &g_editor_state.orig_termios);
    atexit(disableRawMode);

    struct termios raw = g_editor_state.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    checked_tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
