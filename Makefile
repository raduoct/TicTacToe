all: build clean

build: minimax.c
	gcc -Wall -std=c99 minimax.c -o minimax -lm
clean :
	rm -f minimax
	rm -f *.out
