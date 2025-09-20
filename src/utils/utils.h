#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct MemLog {
    char fname[100];
    unsigned int lnum;
} MemLog;

static size_t mlogCount;
static MemLog mlog[50];

static inline void* debug_malloc(size_t size, char* fname, unsigned int lnum)
{
    void* m = malloc(size);
    if (!m) {
        return NULL;
    }

    mlog[mlogCount++] = (MemLog){
        .lnum = lnum,
    };
    memcpy(mlog[mlogCount - 1].fname, fname, strlen(fname));

    printf("%s:L%d\n", fname, lnum);

    return m;
}

#endif // !UTILS_H_
