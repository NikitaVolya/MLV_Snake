#include<stdlib.h>
#include<stdio.h>

#include"snake.h"


int main() {
    vector2i test = create_vector2i(0, 5);

    printf("%d:%d\n", test.x, test.y);

    exit(EXIT_SUCCESS);
}