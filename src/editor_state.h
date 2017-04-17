#ifndef EDITOR_STATE_H
#define EDITOR_STATE_H

#include <termios.h>

typedef struct 
{
    int screenrows;
    int screencols;
    struct termios orig_termios;
} editorState;

extern editorState g_editor_state;

void enableRawMode();
void disableRawMode();

#endif /* EDITOR_STATE_H */
