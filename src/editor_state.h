#ifndef EDITOR_STATE_H
#define EDITOR_STATE_H

#include <termios.h>

typedef struct 
{
    int cx, cy;
    int screenrows;
    int screencols;
    struct termios orig_termios;
} editor_state;

extern editor_state g_editor_state;

void enable_raw_mode();
void disable_raw_mode();

#endif /* EDITOR_STATE_H */
