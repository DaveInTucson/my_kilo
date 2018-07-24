#ifndef EDITOR_STATE_H
#define EDITOR_STATE_H

#include <termios.h>
#include <unistd.h>
#include <time.h>

#define KILO_TAB_STOP 8

typedef struct
{
    int size;
    char *chars;
    int rsize;
    char* render;
} editor_line;

typedef struct 
{
    int cx, cy;
    int rx;
    int screenrows;
    int screencols;

    int line_offset;
    int col_offset;
    int numlines;
    editor_line *lines;
    char *filename;
    int dirty;
    char statusmsg[80];
    time_t statusmsg_time;
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

static inline int get_cursor_rx() { return g_editor_state.rx; }
static inline void set_cursor_rx(int rx) { g_editor_state.rx = rx; }

static inline int get_line_offset() { return g_editor_state.line_offset; }
static inline void set_line_offset(int offset) { g_editor_state.line_offset = offset; }
static inline int get_col_offset() { return g_editor_state.col_offset; }
static inline void set_col_offset(int offset) { g_editor_state.col_offset = offset; }

static inline int get_file_lines() { return g_editor_state.numlines; }

static inline editor_line* get_line(int i)
{ return i < get_file_lines() ? &g_editor_state.lines[i] : NULL ; }

static inline int get_line_size(int i)
{ return i < get_file_lines() ? g_editor_state.lines[i].size : 0; }

static inline char* get_line_chars(int i)
{ return i < get_file_lines() ? g_editor_state.lines[i].chars : NULL; }

static inline int get_render_size(int i)
{ return i < get_file_lines() ? g_editor_state.lines[i].rsize : 0; }

static inline char* get_render_chars(int i)
{ return i < get_file_lines() ? g_editor_state.lines[i].render : NULL; }

void append_file_line(char* line, ssize_t linelen);
void init_file();
void set_filename(const char* filename);
static inline char* get_filename(char * alternate)
{ return g_editor_state.filename != NULL ? g_editor_state.filename : alternate; }

void editor_set_status_message(const char* fmt, ...);
static inline char* get_status_buffer() { return g_editor_state.statusmsg; }
static inline time_t get_status_time() { return g_editor_state.statusmsg_time; }

void editor_insert_char(int c);
void editor_del_char();

static inline void clear_dirty() { g_editor_state.dirty = 0; }
static inline void set_dirty() { g_editor_state.dirty++; }
static inline int  is_dirty() { return g_editor_state.dirty; }

#endif /* EDITOR_STATE_H */
