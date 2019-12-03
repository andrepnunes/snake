CC = gcc


all: out
	./out.out

out: clientAPI snakeAPI snake
	gcc clientAPI.o snakeAPI.o snake.o -o out.out

snake:
	gcc -c $@.c

snakeAPI:
	gcc -c $@.c

clientAPI:
	gcc -c $@.c



.PHONY: clean

clean:
	rm -rf clientAPI.o snake.o  snakeAPI.o *.out