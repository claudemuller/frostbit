#include "vector.h"

vec2_t add_vectors(const vec2_t v1, const vec2_t v2)
{
    return (vec2_t) {
        .x = v1.x + v2.x,
        .y = v1.y + v1.y
    };
}
