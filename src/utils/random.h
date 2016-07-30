#ifndef RDB_RANDOM_H
#define RDB_RANDOM_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

#include <reedb/errors.h>
#include <stdlib.h>

#define RDB_RAND_SUPER_RAND         (1 << 1)
#define RDB_RAND_NORMAL_RAND        (1 << 2)
#define RDB_RAND_WEAK_RAND          (1 << 3)
#define RDB_RAND_SECURE             (1 << 4)

rdb_err_t rdb_rand_genset(char *(*data), size_t length, int flag);

rdb_err_t rdb_rand_nonce(char *(*data), size_t length);

rdb_err_t rdb_rand_gentok(char *(*data));

#ifdef __cplusplus
}
#endif
#endif // RDB_RANDOM_H
