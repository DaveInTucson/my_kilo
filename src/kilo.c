#include "terminal.h"
#include "output.h"
#include "editor_state.h"
#include "die.h"

void initEditor()
{
    if (getWindowSize(&g_editor_state.screenrows, &g_editor_state.screencols) == -1)
        die("getWindowSize");
}

int main()
{
    enableRawMode();
    initEditor();

    while (1)
    {
        editorRefreshScreen();
        editorProcessKeypress(editorReadKey());
    }
    
    return 0;
}
