#include "terminal.h"
#include "output.h"
#include "editor_state.h"
#include "die.h"
#include "file-io.h"

void init_editor()
{
    set_cursor_x(0);
    set_cursor_y(0);
    set_cursor_rx(0);
    init_file();
    
    int rows, cols;
    if (get_window_size(&rows, &cols) == -1)
        die("getWindowSize");

    set_screen_height(rows-2);
    set_screen_width(cols);
}

int main(int argc, char *argv[])
{
    enable_raw_mode();
    init_editor();
    if (argc >= 2)
	editor_open(argv[1]);
    
    editor_set_status_message("HELP: Ctrl-S = Save | Ctrl-Q = quit | Ctrl-F = find");

    while (1)
    {
        editor_refresh_screen();
        editor_process_keypress(editor_read_key());
    }
    
    return 0;
}
