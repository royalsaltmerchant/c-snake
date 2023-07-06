CC := gcc
CFLAGS := -Wall -Wextra -std=c99
LDFLAGS := -lSDL2 -lSDL2_ttf

snake.app: snake
	osacompile -o snake script.applescript

snake: snake.o engine.o
	$(CC) $(CFLAGS) -o snake snake.o engine.o $(LDFLAGS)

snake.o: snake.c engine.h
	$(CC) $(CFLAGS) -c snake.c

engine.o: engine.c engine.h
	$(CC) $(CFLAGS) -c engine.c

.PHONY: clean

clean:
	rm -f snake snake.o engine.o
