
#ifndef VECTOR2I_H
#define VECTOR2I_H

#include<math.h>

struct vector2i
{
    int x, y;
};
typedef struct vector2i vector2i;

/* 
    Creates a 2D integer vector.
    input:    x, y - integer coordinates
    output:   vector2i structure with given coordinates
*/
vector2i create_vector2i(int x, int y);

/* 
    Creates a 2D integer vector with both coordinates set to zero.

    input:    none
    output:   vector2i(0, 0)
*/
vector2i create_zero_vector2i();

/* 
    Adds two 2D integer vectors.

    input:    a, b - vectors to add
    output:   sum of the two vectors
*/
vector2i add_vector2i(vector2i a, vector2i b);

/* 
    Subtracts one 2D integer vector from another.

    input:    a, b - vectors to subtract (a - b)
    output:   result of subtraction
*/
vector2i sub_vector2i(vector2i a, vector2i b);

/* 
    Multiplies a 2D integer vector by an integer scalar.

    input:    vector - source vector
              value  - scalar multiplier
    output:   scaled vector
*/
vector2i mult_vector2i(vector2i vector, int value);

/* 
    Returns a vector with absolute values of both coordinates.

    input:    vector - source vector
    output:   vector with absolute coordinates
*/
vector2i abs_vector2i(vector2i vector);

/* 
    Applies a function to each component of a 2D integer vector and returns the result.

    input:    vector - source vector
              fptr   - pointer to a function that takes and returns an int
    output:   new vector with transformed components
*/
vector2i foreach_vector2i(vector2i vector, int(*fptr)(int));

/*  
    Calculates the vector difference from point a to point b.

    input:
        a - starting point (vector2i)
        b - ending point (vector2i)

    output:
        returns a vector2i representing the difference (b - a)
*/
vector2i get_vector2i_to(vector2i a, vector2i b);

/*  
    Converts a vector into its normalized direction.

    input:
        vector - a vector2i representing a movement or offset

    output:
        returns a vector2i where each component is -1, 0, or 1 
        representing the direction along x and y axes

    behavior:
        - If vector.x or vector.y is zero, the corresponding direction is 0.
        - If vector.x or vector.y is positive, the direction is 1.
        - If vector.x or vector.y is negative, the direction is -1.
        - Useful for iterating step-by-step along a path in a grid.
*/
vector2i get_direction(vector2i vector);

/*  
    Computes the normalized direction vector from point a to point b.

    input:
        a - starting position (vector2i)
        b - target position (vector2i)

    output:
        returns a vector2i with components -1, 0, or 1
        representing the step direction along x and y axes
*/
vector2i get_direction_to(vector2i a, vector2i b);

/*  
    Calculates the Euclidean length (magnitude) of a 2D vector.

    input:
        vector - the input vector (vector2i)

    output:
        returns the length of the vector
*/
float vector2i_get_length(vector2i vector);

/*  
    Calculates the Euclidean distance between two 2D points.

    input:
        a - the first point (vector2i)
        b - the second point (vector2i)

    output:
        returns the distance between points a and b as a floating-point value
*/

float vector2i_get_distance(vector2i a, vector2i b);

#endif