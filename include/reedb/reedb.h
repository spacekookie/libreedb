/* libreedb - reedb.h
 *
 * This file defines all the public core functions for libreedb. From
 * it you will be able to create management instances as well as
 * standalone vaults with the respective runtime and configuration flags.
 * For more information, please consult the official documentation or
 * the repository wiki.
 *
 * (c) 2016 					Lonely Robot.
 * Authors:						Katharina 'spacekookie' Sabel
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * -------------------------------------------
 *
 * A (hopefully) logical API :)
 */


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
    const char          *name, *path;
    size_t              size;
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

rdb_err_t rdb_ctx_free(rdb_context *ctx);

/**
 * Provides the user with a partially initialised vault context.
 *
 * @param ctx An active context for reedb
 * @param vault A pointer to a vault object somewhere
 * @param name The name of the vault on the FS
 * @param path Path of the vault on the FS
 * @return
 */
rdb_err_t rdb_ctx_vaultctr(rdb_context *ctx, rdb_vault *(*vault), const char *name, const char *path);

rdb_err_t rdb_ctx_scpvault(rdb_context *ctx, const char *name, const char *path);

rdb_err_t rdb_ctx_uscpvault(rdb_context *ctx, rdb_uuid *uuid);

/********************************************
 *
 * Vault manipulation functions. Work on a vault provided
 *   by a context. A vault is a collection of records (commonly
 *   referred to as a "database").
 *
 *   Contents of vault are unknown to user because of forward declared scope
 *
 ********************************************/

rdb_err_t rdb_vlts_setflags(rdb_vault *vault, long flags);

/**
 * A zone is a collection of files that a collection of users have
 * access to. Zones can be created during runtime. Each zone has a numeric
 * ID that makes it unique inside it's own vault.
 *
 * Users can then get bound to zone IDs. By default the ID 0 is taken for
 * the root zone.
 *
 * @param vault
 * @param name
 * @return
 */
rdb_err_t rdb_vlts_addzone(rdb_vault *vault, const char *name);

/**
 * Adds a new user to the vault. The name in the vault must be unique.
 *
 * @param name Unique username
 * @param zones A list of zones to add the user to
 * @return The vault user ID
 */
rdb_err_t rdb_vlts_adduser(rdb_vault *vault, const char *name, long zones);

/**
 * Get the user id for a user in this vault. Will return -1 if the user
 * doesn't exist
 *
 * @param username
 * @return
 */
long rdb_vlts_getuser(const char *username);

/**
 * Sets the passphrase for a specific user ID. ID 0 is by default always
 * the ROOT user. This function can only be used to do initial passphrase
 * setup
 *
 * @param user Unique user ID
 * @param passphrase
 */
rdb_err_t rdb_vlts_setlogin(rdb_vault *vault, long user, const char *passphrase);

/**
 * Take currently loaded settings, finalise them and finish setup. Only
 * past this point are changes to the vault final and persistent!
 *
 * This function will throw detailed errors about wrong setup parameters
 */
rdb_err_t rdb_vlts_finalise(rdb_vault *vault);

#ifdef __cplusplus
}
#endif
#endif /* RDB_REEDB_H */