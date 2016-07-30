#include <reedb/reedb.h>

#include <stdlib.h>
#include <stdbool.h>

#include <apr-1/apr_file_io.h>
#include <libconfig.h>

/* CDSL datastructures  */
#include <list.h>
#include <hashtable.h>

/*
    map<rdb_uuid*, rdb_vault*>  *active_vaults;

    class rdb_cfg_wrapper *rdb_scope;

list<class rcry_engine*>    *r_cryptons;

fstream *input, *output;
*/

struct rdb_scope_d {
    unsigned short  valid;
    apr_file_t      *file;
    config_t        *raw;
};

struct ree_context {
    Hashtable       *active;
    List            *cryptons;  // TODO: Change this to a ring queue
    char            *cfg_path;
    char            *log_path;

    /* Scope information */
    struct rdb_scope_d  *scope;
};

/**************************************************************************/
/*************        Forward declaration of functions        *************/
/**************************************************************************/

rdb_err_t load_config(config_t *(*cfg), const char *path);

rdb_err_t save_config(config_t *cfg, const char *path);

rdb_err_t load_scope_file(struct rdb_scope_d *(*scope), const char *path);

rdb_err_t save_scope_file(struct rdb_scope_d *scope, const char *path);

/**************************************************************************/

rdb_err_t rdb_ctx_init(rdb_context *ctx)
{
    if(!ctx) return INVALID_PARAMS;

    memset(ctx, 0, sizeof(rdb_context));

    /* Create our inner context */
    ree_context *inner = (ree_context*) malloc(sizeof(ree_context));
    if(inner == NULL) goto cleanup;

    memset(inner, 0, sizeof(ree_context));

    /* Initialise some sane values */
    inner->scope = (struct rdb_scope_d*) malloc(sizeof(struct rdb_scope_d));
    if(inner->scope == NULL) goto cleanup;

    memset(inner->scope, 0, sizeof(struct rdb_scope_d));

    /* Create the inner data storage */
    inner->active = newHashtable(4);
    inner->cryptons = newList();

    /* Assign inner to outer ctx */
    ctx->inner = inner;
    ctx->active = true;
    ctx->total = 0;

    /* Get the default scope path and attempt to load it */


    return SUCCESS;

    /* Code that gets executed if some malloc fails half way through */
    cleanup:
    printf("Creating context failed...reverting changes\n");
    if(inner->scope) free(inner->scope);
    if(inner) free(inner);

    return MALLOC_FAILED;
}

rdb_err_t rdb_ctx_logpath(rdb_context *ctx, const char *path)
{

}

rdb_err_t rdb_ctx_cfgpath(rdb_context *ctx, const char *path)
{

}

rdb_err_t rdb_ctx_maxscl(rdb_context *ctx, unsigned int scl)
{

}


rdb_err_t rdb_ctx_vaultctr(rdb_context *ctx, rdb_vault *vault, const char *name, const char *path)
{

}

rdb_err_t rdb_ctx_scpvault(rdb_context *ctx, const char *name, const char *path)
{

}

rdb_err_t rdb_ctx_uscpvault(rdb_context *ctx, rdb_uuid *uuid)
{

}

/******************************************************************/

rdb_err_t load_config(config_t *(*cfg), const char *path)
{
    return SUCCESS;
}

rdb_err_t save_config(config_t *cfg, const char *path)
{
    return SUCCESS;
}

rdb_err_t load_scope_file(struct rdb_scope_d *(*scope), const char *path)
{
    return SUCCESS;
}

rdb_err_t save_scope_file(struct rdb_scope_d *scope, const char *path)
{
    return SUCCESS;
}