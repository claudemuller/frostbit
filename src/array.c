#include <stdio.h>
#include <stdlib.h>
#include "array.h"

#define ARRAY_RAW_DATA(array) ((size_t*)(array) - 2)
#define ARRAY_CAPACITY(array) (ARRAY_RAW_DATA(array)[0])
#define ARRAY_OCCUPIED(array) (ARRAY_RAW_DATA(array)[1])

void* array_hold(void* array, size_t count, size_t item_size) {
    if (array == NULL) {
        size_t raw_size = (sizeof(int) * 2) + (item_size * count);
        size_t* base = (size_t*)malloc(raw_size);
        base[0] = count;  // capacity
        base[1] = count;  // occupied
        return base + 2;
    } else if (ARRAY_OCCUPIED(array) + count <= ARRAY_CAPACITY(array)) {
        ARRAY_OCCUPIED(array) += count;
        return array;
    } else {
        size_t needed_size = ARRAY_OCCUPIED(array) + count;
        size_t float_curr = ARRAY_CAPACITY(array) * 2;
        size_t capacity = needed_size > float_curr ? needed_size : float_curr;
        size_t occupied = needed_size;
        size_t raw_size = sizeof(size_t) * 2 + item_size * capacity;
        size_t* base = (size_t*)realloc(ARRAY_RAW_DATA(array), raw_size);
        base[0] = capacity;
        base[1] = occupied;
        return base + 2;
    }
}

size_t array_length(void* array) {
    return (array != NULL) ? ARRAY_OCCUPIED(array) : 0;
}

void array_free(void* array) {
    if (array != NULL) {
        free(ARRAY_RAW_DATA(array));
    }
}
