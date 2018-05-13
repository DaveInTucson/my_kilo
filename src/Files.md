## die.c, die.h

Contains the `die` function, clear the screen, report an error message and exit.

## editor_state.c, editor_state.h

Definition of the `editor_state` struct and `enable_raw_mode`,
`disable_raw_mode` functions.

## kilo.c

`main` function

## output.c, output.h

`editor_refresh_screen` and support functions

## strings.in

Definition of various string constants used by kilo. This is converted to
`string.h` by the `strings2c` Perl script.

## term_buffer.c, term_buffer.h

This implements the `term_buffer` struct and associated functions. It's used
to build up text to be written to the screen, so it can be written in a
single `write` call.

## terminal.c, terminal.h

Terminal access functions