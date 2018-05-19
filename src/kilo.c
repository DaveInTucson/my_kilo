#include "terminal.h"
#include "output.h"
#include "editor_state.h"
#include "die.h"
#include "file-io.h"

void init_editor()
{
    g_editor_state.cx = g_editor_state.cy = 0;
    g_editor_state.numrows = 0;
    
    if (get_window_size(&g_editor_state.screenrows, &g_editor_state.screencols) == -1)
        die("getWindowSize");
}

int main(int argc, char *argv[])
{
    enable_raw_mode();
    init_editor();
    if (argc >= 2)
	editor_open(argv[1]);
    
    while (1)
    {
        editor_refresh_screen();
        editor_process_keypress(editor_read_key());
    }
    
    return 0;
}
