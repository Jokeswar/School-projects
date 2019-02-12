CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJECTS = main.o utils.o task1.o task2.o task3.o simulate_game.o task4.o bonus.o
HEADERS = bmp_header.h utils.h tasks.h
EXEC = main

.PHONY = default build clean

default: run

build: $(EXEC)

run_task1: build
	./$(EXEC) 1

run_task2: build
	./$(EXEC) 2

run_task3: build
	./$(EXEC) 3

run_task4: build
	./$(EXEC) 4

run_bonus: build
	./$(EXEC) 5

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

run: build
	./$(EXEC) ${ARGS}

clean:
	rm -f *.o
	rm -f $(EXEC)