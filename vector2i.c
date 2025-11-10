#include"vector2i.h"


vector2i create_vector2i(int x, int y) {
    vector2i vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

vector2i create_zero_vector2i() {
    return create_vector2i(0, 0);
}

vector2i add_vector2i(vector2i a, vector2i b) {
    vector2i rep;
    rep.x = a.x + b.x;
    rep.y = a.y + b.y;
    return rep;
}

vector2i sub_vector2i(vector2i a, vector2i b) {
    vector2i rep;
    rep.x = a.x - b.x;
    rep.y = a.y - b.y;
    return rep;
}

vector2i mult_vector2i(vector2i vector, int value) {
    vector2i rep;
    rep.x = vector.x * value;
    rep.y = vector.y * value;
    return rep;
}


vector2i abs_vector2i(vector2i vector) {
    vector2i rep;
    rep.x = (vector.x < 0 ? -vector.x : vector.x);
    rep.y = (vector.y < 0 ? -vector.y : vector.y);
    return rep;
}

vector2i foreach_vector2i(vector2i vector, int(*fptr)(int)) {
    vector2i rep;
    rep.x = fptr(vector.x);
    rep.y = fptr(vector.y);
    return rep;
}

vector2i get_vector2i_to(vector2i a, vector2i b) {
    vector2i res;

    res.x = b.x - a.x;
    res.y = b.y - a.y;

    return res;
}

vector2i get_direction(vector2i vector) {
    vector2i res;

    if (vector.x == 0)
        res.x = 0;
    else if (vector.x < 0)
        res.x = -1;
    else
        res.x = 1;

    if (vector.y == 0)
        res.y = 0;
    else if (vector.y < 0)
        res.y = -1;
    else
        res.y = 1;

    return res;
}

vector2i get_direction_to(vector2i a, vector2i b) {
    return get_direction(get_vector2i_to(a, b));
}

float vector2i_get_length(vector2i vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float vector2i_get_distance(vector2i a, vector2i b) {
    return vector2i_get_length(get_vector2i_to(a, b));
}