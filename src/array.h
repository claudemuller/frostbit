#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#define array_push(array, value)                                              \
    do {                                                                      \
        (array) = array_hold((array), 1, sizeof(*(array)));                   \
        (array)[array_length(array) - 1] = (value);                           \
    } while (0);

void* array_hold(void* array, size_t count, size_t item_size);
size_t array_length(void* array);
void array_free(void* array);

#endif
