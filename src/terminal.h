#ifndef TERMINAL_H
#define TERMINAL_H

typedef int keypress_t;

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

void enable_raw_mode();
keypress_t editor_read_key();
int  get_window_size(int *rows, int *cols);
void editor_process_keypress(keypress_t);
char *editor_prompt(char *prompt, void (*callback)(char *, int));

#endif /* TERMINAL_H */
