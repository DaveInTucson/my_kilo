#ifndef TERMINAL_H
#define TERMINAL_H

void enable_raw_mode();
char editor_read_key();
int  get_window_size(int *rows, int *cols);
void editor_process_keypress(char);

#endif /* TERMINAL_H */
