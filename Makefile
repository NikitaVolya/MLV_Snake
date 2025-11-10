snake_game : main.o vector2i.o snake.o
	gcc -o snake_game  main.o vector2i.o snake.o -lm
main.o : main.c
	gcc -o main.o -W -Wall -pedantic -std=c89 -O2 -c main.c
snake.o : snake.h snake.c vector2i.h
	gcc -o snake.o -W -Wall -pedantic -std=c89 -O2 -c snake.c
vector2i.o : vector2i.o vector2i.c
	gcc -o vector2i.o -W -Wall -pedantic -std=c89 -O2 -c vector2i.c
clean :
	rm -rf *.o snake_game