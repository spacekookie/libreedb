#ifndef RDB_DATA_H
#define RDB_DATA_H

#include <memory.h>

#include <reedb/errors.h>

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UNSET, LITERAL, NUMERAL, RECURSIVE
} rdb_uni_t;

typedef struct rdb_data {
    rdb_uni_t       type;
    size_t          size, used;
    union {
        char                *literal;
        int                 numeral;
        struct rdb_data     *(*recursive);
    } payload;
} rdb_data;

/** Malloc a new rdb_data object */
rdb_err_t rdb_data_malloc(rdb_data *(*data));

rdb_err_t rdb_data_resettype(rdb_data *data);

/** Set the data element to a literal and save it's length */
rdb_err_t rdb_data_addliteral(rdb_data *data, const char *literal, size_t length);

/** Set the data element to a numeral */
rdb_err_t rdb_data_addnumeral(rdb_data *data, int numeral);

/** Add a new data element to the resursive data store */
rdb_err_t rdb_data_mallocrecursive(rdb_data *data, rdb_data *(*new_data));

const char *rdb_data_dtype(rdb_uni_t type);

/** Will free all memory allocated by this element and it's children */
rdb_err_t rdb_data_free(rdb_data *data);

#ifdef __cplusplus
}
#endif
#endif //REEDB_DATA_H
