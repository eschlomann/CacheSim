# Makefile for the project

CC = gcc
CFLAGS = -I.
DEPS = cache.h configparse.h main.h print.h
OBJ = cache.c configparse.c main.c print.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cacheSim.exe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
