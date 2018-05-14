#ifndef TERMINAL_H
#define TERMINAL_H

typedef int keypress_t;

void enable_raw_mode();
keypress_t editor_read_key();
int  get_window_size(int *rows, int *cols);
void editor_process_keypress(keypress_t);

#endif /* TERMINAL_H */
