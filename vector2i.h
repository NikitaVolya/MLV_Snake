/**
 * @file vector2i.h
 * @brief Utility functions and structures for working with 2D integer vectors.
 *
 * This file provides a simple 2D integer vector type (`vector2i`) and
 * common operations such as addition, subtraction, scaling, normalization,
 * distance calculation, and component-wise transformations.  
 *
 * These helpers are designed for use in grid-based and coordinate-based systems,
 * such as games, physics simulations, and general spatial computations.
 */

#ifndef _VECTOR2I_H_
#define _VECTOR2I_H_

#include<math.h>


/**
 * @struct vector2i
 * @brief Represents a 2D integer vector.
 */
struct vector2i
{
    int x; /**< X coordinate */
    int y; /**< Y coordinate */
};
typedef struct vector2i vector2i;

/**
 * @brief Creates a 2D integer vector with given coordinates.
 *
 * @param[in] x X coordinate.
 * @param[in] y Y coordinate.
 * @return A vector2i containing the specified coordinates.
 */
vector2i create_vector2i(int x, int y);

/**
 * @brief Creates a 2D vector with coordinates set to (0, 0).
 *
 * @return A zero vector (0, 0).
 */
vector2i create_zero_vector2i();

/**
 * @brief Adds two 2D integer vectors.
 *
 * @param[in] a First vector.
 * @param[in] b Second vector.
 * @return The sum of both vectors (a + b).
 */
vector2i add_vector2i(vector2i a, vector2i b);

/**
 * @brief Subtracts vector b from vector a.
 *
 * @param[in] a Minuend vector.
 * @param[in] b Subtrahend vector.
 * @return The result of subtraction (a - b).
 */
vector2i sub_vector2i(vector2i a, vector2i b);

/**
 * @brief Multiplies a vector by an integer scalar.
 *
 * @param[in] vector Input vector.
 * @param[in] value Integer multiplier.
 * @return The scaled vector.
 */
vector2i mult_vector2i(vector2i vector, int value);

/**
 * @brief Converts both coordinates to their absolute values.
 *
 * @param[in] vector Input vector.
 * @return The absolute-value version of the vector.
 */
vector2i abs_vector2i(vector2i vector);

/**
 * @brief Applies a transformation function to each coordinate of a vector.
 *
 * @param[in] vector Input vector.
 * @param[in] fptr Pointer to a function taking an `int` and returning an `int`.
 * @return A new vector with both coordinates transformed by the function.
 */
vector2i foreach_vector2i(vector2i vector, int(*fptr)(int));


/**
 * @brief Computes the vector difference from point a to point b.
 *
 * @param[in] a Starting point.
 * @param[in] b Ending point.
 * @return A vector representing the difference (b - a).
 */
vector2i get_vector2i_to(vector2i a, vector2i b);

/**
 * @brief Normalizes the vector into direction components (-1, 0, or 1).
 *
 * @param[in] vector Input vector.
 * @return A direction vector where each component is -1, 0, or 1.
 *
 * @details
 * - If vector.x > 0 → direction.x = 1  
 * - If vector.x < 0 → direction.x = -1  
 * - If vector.x = 0 → direction.x = 0  
 *
 * The same rules apply to the Y coordinate.
 */
vector2i get_direction(vector2i vector);

/**
 * @brief Returns the normalized direction from point a to point b.
 *
 * @param[in] a Starting point.
 * @param[in] b Target point.
 * @return A vector with each component equal to -1, 0, or 1.
 */
vector2i get_direction_to(vector2i a, vector2i b);

/**
 * @brief Computes the Euclidean length (magnitude) of a vector.
 *
 * @param[in] vector Input vector.
 * @return The length of the vector as a float.
 */
float vector2i_get_length(vector2i vector);


/**
 * @brief Computes the Euclidean distance between two 2D points.
 *
 * @param[in] a First point.
 * @param[in] b Second point.
 * @return The distance between the two points.
 */
float vector2i_get_distance(vector2i a, vector2i b);

#endif /* _VECTOR2I_H_ */