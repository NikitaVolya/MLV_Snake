snake_game : main.o vector2i.o snake.o game_config.o
	gcc -o snake_game  main.o vector2i.o snake.o -lm
main.o : main.c
	gcc -o main.o -W -Wall -pedantic -std=c89 -O2 -c main.c
game_config.o : game_config.h game_config.c vector2i.h
	gcc -o game_config.o -W -Wall -pedantic -std=c89 -O2 -c game_config.c
snake.o : snake.h snake.c vector2i.h
	gcc -o snake.o -W -Wall -pedantic -std=c89 -O2 -c snake.c
vector2i.o : vector2i.o vector2i.c
	gcc -o vector2i.o -W -Wall -pedantic -std=c89 -O2 -c vector2i.c
clean :
	rm -rf *.o snake_game