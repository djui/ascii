CC=gcc

all: klarna yahoo fire

klarna:
	$(CC) -o bin/klarna -std=c99 src/klarna.c

yahoo:
	$(CC) -o bin/yahoo -std=c99 src/yahoo.c

fire:
	$(CC) -o bin/fire src/fire.c
