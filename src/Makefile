OBJ=kilo.o terminal.o die.o output.o editor_state.o term_buffer.o
CFLAGS=-Wall -Wextra -pedantic -std=c99
STRINGS2C=../bin/strings2c

kilo: ${OBJ}
	$(CC) ${OBJ} -o kilo

kilo.o: kilo.c terminal.h output.h editor_state.h die.h

terminal.o: terminal.c terminal.h die.h editor_state.h

die.o : die.c die.h strings.h

output.o: output.c output.h strings.h editor_state.h term_buffer.h

editor_state.o: editor_state.c editor_state.h die.h

term_buffer.o: term_buffer.h strings.h die.h

strings.h: strings.in
	${STRINGS2C} strings.in

clean:
	rm -f *~ *.o kilo
