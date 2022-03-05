
CC = gcc
CFLAGS = -g -W -Wall -ansi
LDFLAGS =
LIBS = -ldl
OBJS = malloc.o

all: malloc

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

malloc: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
.PHONY: clean
clean:
	rm -rf malloc
	find . -name "*.o" -exec rm {} \;
