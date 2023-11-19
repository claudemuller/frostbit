#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct {
	float x;
	float y;
} vec2_t;

vec2_t add_vectors(const vec2_t v1, const vec2_t v2);

#endif // VECTOR_H_
