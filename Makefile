snake_game : main.o vector2i.o
	gcc -o snake_game  main.o vector2i.o -lm
main.o : main.c vector2i.c
	gcc -o main.o -W -Wall -pedantic -std=c89 -O2 -c main.c
vector2i.o : vector2i.o vector2i.c
	gcc -o vector2i.o -W -Wall -pedantic -std=c89 -O2 -c vector2i.c
clean :
	rm -rf *.o snake_game