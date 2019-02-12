CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
OBJECTS = main.o wrap.o util.o alignLeft.o alignRight.o paragraph.o center.o list.o orderedList.o justify.o
HEADERS = wrap.h util.h alignLeft.h alignRight.h paragraph.h center.h list.h orderedList.h justify.h
EXEC = format_text

.PHONY = default build clean

default: run

build: $(EXEC)

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

run: build
	./$(EXEC) ${ARGS}

clean:
	rm -f *.o
	rm -f $(EXEC)