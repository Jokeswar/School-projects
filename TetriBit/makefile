CC = gcc
CFLAGS = -Wall -lm -std=c99
SRC = main.c
EXE = tema1

run: build
	./$(EXE)

build:
	$(CC) $(SRC) -o $(EXE) $(CFLAGS)

clean:
	rm -f $(EXE)
