#include "die.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "strings.h"

void die(const char* message)
{
    string_const cs = get_clear_screen_str();
    write(STDOUT_FILENO, cs.s, cs.length);

    string_const hc = get_home_cursor_str();
    write(STDOUT_FILENO, hc.s, hc.length);

    perror(message);
    exit(1);
}
