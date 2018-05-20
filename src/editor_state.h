#ifndef EDITOR_STATE_H
#define EDITOR_STATE_H

#include <termios.h>
#include <unistd.h>

typedef struct
{
    int size;
    char *chars;
} editor_line;

typedef struct 
{
    int cx, cy;
    int screenrows;
    int screencols;

    int numlines;
    editor_line line;
    struct termios orig_termios;
} editor_state;

extern editor_state g_editor_state;

void enable_raw_mode();
void disable_raw_mode();

static inline int get_screen_width() { return g_editor_state.screencols; }
static inline int get_screen_height() { return g_editor_state.screenrows; }

static inline void set_screen_width(int width) 
{ g_editor_state.screencols = width; }

static inline void set_screen_height(int height)
{ g_editor_state.screenrows = height; }

static inline int get_cursor_x() { return g_editor_state.cx; }
static inline int get_cursor_y() { return g_editor_state.cy; }

static inline void set_cursor_x(int cx) { g_editor_state.cx = cx; }
static inline void set_cursor_y(int cy) { g_editor_state.cy = cy; }

static inline int get_file_lines() { return g_editor_state.numlines; }
static inline void clear_file() { g_editor_state.numlines = 0; }

static inline int get_line_size() { return g_editor_state.line.size; }
static inline char* get_line_chars() { return g_editor_state.line.chars; }

void add_file_line(char* line, ssize_t linelen);

#endif /* EDITOR_STATE_H */
