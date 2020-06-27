CC = gcc
KERNEL = $(shell uname -r)
LIBS = -lm
CFLAGS = -Wall -I. -I/usr/include/x86_64-linux-gnu
OBJECTS = common.o cli.o tty.o output.o input.o
PREFIX := /usr/local
EXEC := upper

upper: $(OBJECTS) main.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

common.o: common.c common.h
cli.o: cli.c cli.h common.h 
tty.o: tty.c tty.h common.h
output.o: output.c output.h tty.h common.h
input.o: input.c input.h common.h

.PHONY: clean 
clean:
	rm -f $(OBJECTS) $(EXEC)


.PHONY: install
install: $(EXEC)
	install -m 755 $(EXEC) $(DESTDIR)$(PREFIX)/bin


.PHONY: test
test: $(EXEC)
	./upper -v

