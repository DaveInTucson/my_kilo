#include "terminal.h"
#include "output.h"
#include "editor_state.h"
#include "die.h"

void init_editor()
{
    g_editor_state.cx = g_editor_state.cy = 0;

    if (get_window_size(&g_editor_state.screenrows, &g_editor_state.screencols) == -1)
        die("getWindowSize");
}

int main()
{
    enable_raw_mode();
    init_editor();

    while (1)
    {
        editor_refresh_screen();
        editor_process_keypress(editor_read_key());
    }
    
    return 0;
}
