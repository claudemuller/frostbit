#ifndef UTILS_H_
#define UTILS_H_

#include <SDL3/SDL_log.h>
#include <limits.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#define util_error(fmt, ...) util_err("ERROR [%s:%d]: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define util_info(fmt, ...) util_inf("INFO: " fmt, ##__VA_ARGS__)
#define util_warn(fmt, ...) util_inf("WARN [%s:%d]: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define util_debug(fmt, ...) util_inf("DEBUG [%s:%d]: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#pragma clang diagnostic pop

#define MAX_MEM_LOGS 1024
#define PATH_MAX 255

typedef struct {
    void* ptr;
    char fname[PATH_MAX];
    unsigned int lnum;
    int used;
} MemLog;

#define DBG_MALLOC(sz) debug_malloc((sz), __FILE__, __LINE__)
#define DBG_FREE(p) debug_free((p), __FILE__, __LINE__)

void util_inf(const char* fmt, ...);
void util_err(const char* fmt, ...);
void* debug_malloc(size_t size, const char* fname, unsigned int lnum);
void debug_free(void* ptr, const char* fname, unsigned int lnum);

#endif // UTILS_H_
