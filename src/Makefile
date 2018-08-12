OBJ=kilo.o terminal.o die.o output.o editor_state.o term_buffer.o file-io.o dlog.o find.o
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g
STRINGS2C=../bin/strings2c

kilo: ${OBJ}
	$(CC) ${OBJ} -o kilo

kilo.o: kilo.c terminal.h output.h editor_state.h die.h file-io.h

terminal.o: terminal.c terminal.h die.h editor_state.h strings.h file-io.h output.h find.h

die.o : die.c die.h strings.h

output.o: output.c output.h strings.h editor_state.h term_buffer.h dlog.h

editor_state.o: editor_state.c editor_state.h die.h

term_buffer.o: term_buffer.h strings.h die.h dlog.h

file-io.o: file-io.c file-io.h editor_state.h die.h dlog.h

find.o: find.c editor_state.h terminal.h output.h

strings.h: strings.in
	${STRINGS2C} strings.in

dlog.o: dlog.c dlog.h

clean:
	rm -f *~ *.o kilo
