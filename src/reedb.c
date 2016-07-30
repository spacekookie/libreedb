#include <reedb/reedb.h>
#include <stdlib.h>

#include <gdsl_hash.h>
#include <gdsl_list.h>

/*
    map<rdb_uuid*, rdb_vault*>  *active_vaults;

    class rdb_cfg_wrapper *rdb_scope;

list<class rcry_engine*>    *r_cryptons;

fstream *input, *output;
*/
struct ree_context {
    gdsl_hash_t     *active;
};

rdb_err_t rdb_ctx_init(rdb_context *ctx)
{
    if(!ctx) return INVALID_PARAMS;

    memset(ctx, 0, sizeof(rdb_context));

    /* Create our inner context */
    ree_context *inner = (ree_context*) malloc(sizeof(ree_context));
    memset(inner, 0, sizeof(ree_context));

    /* Initialise some sane values */

    /* Assign inner to outer ctx */
    ctx->inner = inner;
    ctx->active = true;
    ctx->total = 0;
}

rdb_err_t rdb_ctx_logpath(rdb_context *ctx, const char *path)
{

}

rdb_err_t rdb_ctx_cfgpath(rdb_context *ctx, const char *path)
{

}

rdb_err_t rdb_ctx_verbose(rdb_context *ctx, bool verbose)
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
