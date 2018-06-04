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

    int line_offset;
    int col_offset;
    int numlines;
    editor_line *lines;
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

static inline int get_line_offset() { return g_editor_state.line_offset; }
static inline void set_line_offset(int offset) { g_editor_state.line_offset = offset; }
static inline int get_col_offset() { return g_editor_state.col_offset; }
static inline void set_col_offset(int offset) { g_editor_state.col_offset = offset; }

static inline int get_file_lines() { return g_editor_state.numlines; }

static inline int get_line_size(int i)
{ return i < get_file_lines() ? g_editor_state.lines[i].size : 0; }

static inline char* get_line_chars(int i)
{ return i < get_file_lines() ? g_editor_state.lines[i].chars : NULL; }

void append_file_line(char* line, ssize_t linelen);
void init_file();

#endif /* EDITOR_STATE_H */
