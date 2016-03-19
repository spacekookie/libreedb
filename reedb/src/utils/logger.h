#ifndef _LOGGING_H
#define _LOGGING_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RDB_DEFAULT_LOGLEVEL DEBUG;

enum rdb_loglevel {
    LOG_NONE = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
};

typedef enum rdb_loglevel rdb_loglevel_t;

rdb_loglevel_t loglevel = RDB_DEFAULT_LOGLEVEL ;

const char *const LOG_LEVEL_NAMES[] = {
        "NONE",
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG",
        "CUPCAKE"
};

#define rdb_levelname(M) LOG_LEVEL_NAMES[M]

#ifdef NDEBUG
#define rdb_debug(M, ...)
#else
#define rdb_debug(M, ...) rdb_logline(LOG_DEBUG, 0, M, ##__VA_ARGS__)
#endif

#define rdb_clean_errno() (errno == 0 ? "None" : strerror(errno))

#define rdb_logline(L, E, M, ...) { \
    char date[20]; \
    struct timeval tv; \
    gettimeofday(&tv, NULL); \
    strftime(date, sizeof(date) / sizeof(*date), "%Y-%m-%dT%H:%M:%S", gmtime(&tv.tv_sec)); \
    if ( E ) { \
        fprintf(stderr, "%s.%03dZ [%s] (%s:%d: errno: %s) " M "\n", &date[0], (tv.tv_usec/1000), rdb_levelname( L ), __FILE__, __LINE__, rdb_clean_errno(), ##__VA_ARGS__); \
    } else { \
        fprintf(stderr, "%s.%03dZ [%s] (%s:%d) " M "\n", &date[0], (tv.tv_usec/1000), rdb_levelname( L ), __FILE__, __LINE__, ##__VA_ARGS__); \
    } \
} \

#define rdb_log_err(M, ...) if ( LOG_ERROR <= loglevel ) rdb_logline(LOG_ERROR, 1, M , ##__VA_ARGS__)

#define rdb_log_warn(M, ...) if ( LOG_WARN <= loglevel ) rdb_logline(LOG_WARN, 1, M , ##__VA_ARGS__)

#define rdb_log_info(M, ...) if ( LOG_INFO <= loglevel ) rdb_logline(LOG_INFO, 0, M , ##__VA_ARGS__)

#define rdb_check(A, M, ...) if(!(A)) { rdb_log_err(M, ##__VA_ARGS__); errno=0; goto rdb_error; }

#define rdb_sentinel(M, ...)  { rdb_log_err(M, ##__VA_ARGS__); errno=0; goto rdb_error; }

#define rdb_check_mem(A) rdb_check((A), "Out of memory.")

#define rdb_check_debug(A, M, ...) if(!(A)) { rdb_debug(M, ##__VA_ARGS__); errno=0; goto error; }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LOGGING_H
