#ifndef UTILS_H_
#define UTILS_H_

#include <SDL3/SDL_log.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef struct MemLog {
    char fname[100];
    unsigned int lnum;
} MemLog;

static size_t mlogCount;
static MemLog mlog[50];

#define util_error(fmt, ...) util_err("ERROR [%s:%d]: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define util_info(fmt, ...) util_inf("INFO: " fmt, ##__VA_ARGS__)
#define util_debug(fmt, ...) util_inf("DEBUG [%s:%d]: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

static inline void util_inf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ap);

    va_end(ap);
}

static inline void util_err(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, ap);

    va_end(ap);
}

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

    util_info("malloc at %s:L%d\n", fname, lnum);

    return m;
}

#endif // !UTILS_H_
