
/* Make sure we're not included multiple times */
#ifndef RDB_ERRORS_H
#define RDB_ERRORS_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

/** Define some generic error codes first that we can propagate **/
typedef enum rdb_err_t {

    /* General purpose error codes */
    FAILURE = -1,                   // To be used when cause of error not known.
    SUCCESS = 0,                    // When something went according to plan.
    HUGE_SUCCESS = 0,               // Little Portal easter egg :)

    INVALID_PARAMS,
    MALLOC_FAILED,                  // A memory allocation couldn't be completed!
    INVALID_PAYLOAD,                // A common data return problem
    INVALID_BUFFER_SIZE,

} rdb_err_t;

const char *rdb_error_getmsg(rdb_err_t *e);

#ifdef __cplusplus
}
#endif
#endif /* RDB_ERRORS_H */