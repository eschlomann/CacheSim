# Makefile for the project

CC = gcc
CFLAGS = -I.
DEPS = cache.h configparse.h main.h print.h statemachine.h
OBJ = cache.o configparse.o main.o print.o statemachine.o
LIBS = -lm

%.o: %.c $(DEPS)
	$(CC) -O3 -Wall -Wextra -c -o $@ $< $(CFLAGS)

cacheSim.exe: $(OBJ)
	$(CC) -O3 -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
	rm cacheSim.exe
