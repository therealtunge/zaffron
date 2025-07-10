CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Iinclude -I/mingw64/include -O3 -std=c2x -g
LDFLAGS= -L/mingw64/bin -lSDL3

SOURCES=$(wildcard *.c utils/*.c cpu/*.c emu/*.c render/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=exec.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(EXECUTABLE) $(OBJECTS)
