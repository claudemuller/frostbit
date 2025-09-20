#ifndef UTILS_H_
#define UTILS_H_

#include <SDL3/SDL_log.h>
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

/**
 * A log printing function that wraps the current library of the day.
 */
static inline void util_log(const char* pre, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    size_t fmtlen = strlen(fmt);
    char* f = (char*)malloc(sizeof(char) * fmtlen + 3);

    memcpy(f, "%s", 2);
    memcpy(f + 2, fmt, fmtlen);
    f[fmtlen + 2] = '\0';

    SDL_Log(f, pre, args);
    free(f);

    va_end(args);
}

static inline void util_info(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    util_log("LOG: ", fmt, args);

    va_end(args);
}

static inline void util_error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    util_log("ERROR: ", fmt, args);

    va_end(args);
}

#endif // !UTILS_H_
