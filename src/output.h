#ifndef OUTPUT_H
#define OUTPUT_H

#include "editor_state.h"

extern void editor_refresh_screen();
extern int editor_row_rx_to_cx(editor_line *line, int rx);

#endif /* OUTPUT_H */
