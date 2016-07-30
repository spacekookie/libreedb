
/* Make sure we're not included multiple times */
#ifndef RDB_REEDB_H
#define RDB_REEDB_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

// Other parts of the public facing reedb front
#include <reedb/errors.h>

// Generic runtime includes
#include <apr-1/apr_time.h>
#include <stdbool.h>

/** Some flags that can be used to configure a vault after creation **/
#define RDB_FLG_ROOT                    (1 << 0)
#define RDB_FLG_MULTI_USER              (1 << 1)
#define RDB_FLG_MANAGED                 (1 << 2)
#define RDB_FLG_ANARCHY                 (1 << 3)

#define RDB_FLG_CACHE_SECURE            (1 << 4)
#define RDB_FLG_CACHE_QUICK             (1 << 5)
#define RDB_FLG_CACHE_BUFFERED          (1 << 6)

#define RDB_FLG_WRITE_BLOCK             (1 << 7)
#define RDB_FLG_WRITE_TREE              (1 << 8)

#define RDB_FLG_SCALE_DYNAMIC           (1 << 9)
#define RDB_FLG_SCALE_ENDLESS           (1 << 10)

#define RDB_FLG_LOG_VERBOSE             (1 << 11)
#define RDB_FLG_LOG_INFO                (1 << 12)
#define RDB_FLG_LOG_WARN                (1 << 13)
#define RDB_FLG_LOG_ERROR               (1 << 14)
#define RDB_FLG_LOG_SILENT              (1 << 15)

#define RDB_FLG_FTR_SINGLE_RECORD       (1 << 16)
#define RDB_FLG_FTR_MULTI_RECORD        (1 << 17)
#define RDB_FLG_FTR_RECORD_TREE         (1 << 18)
#define RDB_FLG_FTR_DISABLE_LUT         (1 << 19)
#define RDB_FLG_FTR_DISABLE_RQL         (1 << 20)
#define RDB_FLG_FTR_PERMISSIVE          (1 << 21)
#define RDB_FLG_FTR_DISABLE_HEADERS     (1 << 22)

// #define RDB_FLAG_RQLSYNTAX_SIMPLE       (1 << 23)

#define RDB_ZONE_ROOT                   0x00000000
#define RDB_USER_ROOT                   0x00000000

#define REAL_STRLEN(string) (strlen(string) + 1)

/********************************************
 *
 * Basic structs to interact with reedb. Tokens, uuids,
 * vaults and base context.
 *
 ********************************************/

/** Metadata structs for user communication **/
typedef struct rdb_token {
    char            *contents;
    size_t          size;
    apr_time_t      created;
    unsigned int    timed;
} rdb_token;

/** UUID handle to identify vaults internally and to the user */
typedef struct rdb_uuid { unsigned char x[32]; } rdb_uuid;

/* Contains metadata about a vault returned to the dev */
typedef struct {
    rdb_uuid        id;
    char            *name, *path;
    uint64_t        size;
    bool            active;
} vault_meta;

/* Contains metadata fields about a file */
typedef struct {
    unsigned int    revisions;
    char            *category;
    char            *title;
    char            **tags;
    size_t          no_tags;
} file_meta;

/** Inner context to hold sensitive data */
typedef struct ree_context ree_context;

typedef struct rdb_context {
    struct ree_context  *inner; // Hide stuff from users!
    size_t              active, total;
} rdb_context ;

typedef struct ree_vault ree_vault;

typedef struct rdb_vault {
    struct ree_vault    *inner;
    char                **zones;
    size_t              no_zones;
} rdb_vault;

/********************************************
 *
 * Context manipulation functions. Work on a user context
 *   that is then passed around other parts of the API to
 *   provice a static context across all components.
 *
 *   Contents of context are unknown to user because of forward declared scope
 *
 ********************************************/

rdb_err_t rdb_ctx_init(rdb_context *ctx);

rdb_err_t rdb_ctx_logpath(rdb_context *ctx, const char *path);

rdb_err_t rdb_ctx_cfgpath(rdb_context *ctx, const char *path);

rdb_err_t rdb_ctx_verbose(rdb_context *ctx, bool verbose);

rdb_err_t rdb_ctx_maxscl(rdb_context *ctx, unsigned int scl);

/**
 * Provides the user with a partially initialised vault context.
 *
 * @param ctx An active context for reedb
 * @param vault A pointer to a vault object somewhere
 * @return
 */
rdb_err_t rdb_ctx_vaultctr(rdb_context *ctx, rdb_vault *vault);

rdb_err_t rdb_ctx_scpvault(rdb_context *ctx, const char *name, const char *path);

rdb_err_t rdb_ctx_uscpvault(rdb_context *ctx, rdb_uuid *uuid);

#ifdef __cplusplus
}
#endif
#endif /* RDB_REEDB_H */