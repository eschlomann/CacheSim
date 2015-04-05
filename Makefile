# Makefile for the project

CC = gcc
CFLAGS = -I.
DEPS = cache.h configparse.h main.h print.h
OBJ = cache.o configparse.o main.o print.o
LIBS = -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cacheSim.exe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
	rm cacheSim.exe
