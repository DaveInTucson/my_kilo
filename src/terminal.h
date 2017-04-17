#ifndef TERMINAL_H
#define TERMINAL_H

void enableRawMode();
char editorReadKey();
int  getWindowSize(int *rows, int *cols);
void editorProcessKeypress(char);

#endif /* TERMINAL_H */
