#include <reedb/reedb.h>

#include <reedb/platform.h>
#include <reedb/data.h>

#include <stdlib.h>
#include <stdbool.h>

#include <apr-1/apr_file_io.h>

#include <libconfig.h>

#include <hashtable.h>
#include <wordexp.h>
#include <list.h>

// System functions
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "config.h"


#include <stb_leakcheck.h>

/*
    map<rdb_uuid*, rdb_vault*>  *active_vaults;

    class rdb_cfg_wrapper *rdb_scope;

list<class rcry_engine*>    *r_cryptons;

fstream *input, *output;
*/

struct rdb_scope_d {
    unsigned short  valid;
    rdb_data        *scoped;
};

struct ree_context {
    Hashtable       *active;
    List            *cryptons;  // TODO: Change this to a ring queue
    char            *cfg_path;
    char            *log_path;
    char            *dir_path;

    /* Config file for this instance */
    config_t        *cfg;

    /* Scope information */
    struct rdb_scope_d  *scope;
};

struct ree_vault {
    unsigned long       type;
    unsigned long       cache;
    unsigned long       write_m;
    unsigned long       scale_t;
    unsigned long       log_t;
    unsigned long       ftr_single, ftr_multi, ftr_tree;
    unsigned long       ftr_lut_dis, ftr_rql_dis;
    unsigned long       ftr_perm;
    unsigned long       ftr_head_dis;

    /* Vault contents */
    Hashtable           *headers, *files;
    List                *tags, *categories;
};

/**************************************************************************/
/*************        Forward declaration of functions        *************/
/**************************************************************************/

rdb_err_t load_config(config_t *(*cfg), const char *path);

rdb_err_t save_config(config_t *cfg, const char *path);

rdb_err_t populate_paths(char *(*cfg_path), char *(*log_path), char *(*dir_path));

rdb_err_t create_newconfig(config_t **cfg);

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

    /* Call a function to populate the path variables for us: platform agnosticly! */
    rdb_err_t err = populate_paths(&ctx->inner->cfg_path, // Confi file path
                                     &ctx->inner->log_path, // Log file path (unused)
                                     &ctx->inner->dir_path);
    if(err) return err;

    if( access(ctx->inner->cfg_path, F_OK ) != -1 ){

        load_config(&ctx->inner->cfg, ctx->inner->cfg_path);

    } else {
        /* Call our config create function because it would bloat up this function too much */
        create_newconfig(&ctx->inner->cfg);

        /** Save the confic */
        save_config(ctx->inner->cfg, ctx->inner->cfg_path);

    }

    return SUCCESS;

    /* Code that gets executed if some malloc fails half way through */
    cleanup:
    printf("Creating context failed...reverting changes\n");
    if(inner->scope) free(inner->scope);
    free(inner);

    return MALLOC_FAILED;
}

rdb_err_t rdb_ctx_logpath(rdb_context *ctx, const char *path)
{
//    free(ctx->inner->cfg_path);
//
//    ctx->inner->cfg_path = (char*) malloc(sizeof(char) * REAL_STRLEN(path));
//    strcpy(ctx->inner->cfg_path, path);
//
//    /* Then call the setup function again */
//    populate_paths(&ctx->inner->cfg_path, &ctx->inner->log_path);
}

rdb_err_t rdb_ctx_cfgpath(rdb_context *ctx, const char *path)
{
    return NOT_IMPLEMENTED;
}

rdb_err_t rdb_ctx_maxscl(rdb_context *ctx, unsigned int scl)
{
    return NOT_IMPLEMENTED;
}


rdb_err_t rdb_ctx_vaultctr(rdb_context *ctx, rdb_vault *(*vault), const char *name, const char *path)
{

    /* Add the vault to the scope of this context */
    rdb_err_t err = rdb_ctx_scpvault(ctx, name, path);
    if(err) return err;

    (*vault) = (rdb_vault*) malloc(sizeof(rdb_vault));
    if(*vault == NULL) return MALLOC_FAILED;

    (*vault)->inner = (ree_vault*) malloc(sizeof(ree_vault));
    if((*vault)->inner == NULL) return MALLOC_FAILED;



    return SUCCESS;
}

rdb_err_t rdb_ctx_scpvault(rdb_context *ctx, const char *name, const char *path)
{
    config_setting_t *setting;
    config_setting_t *scope = config_lookup(ctx->inner->cfg, CFG_SCOPE);

    config_setting_t *scoped = config_setting_get_member(scope, name);
    if(scoped) {
        printf("Vault already exists in scope!\n");
        return ALREADY_SCOPED;
    }

    config_setting_t *us = config_setting_add(scope, name, CONFIG_TYPE_GROUP);

    /* Add the UUID  */
    setting = config_setting_add(us, CFG_SCOPE_NAME, CONFIG_TYPE_STRING);
    config_setting_set_string(setting, name);


    setting = config_setting_add(us, CFG_SCOPE_NAME, CONFIG_TYPE_STRING);
    config_setting_set_string(setting, name);

    setting = config_setting_add(us, CFG_SCOPE_PATH, CONFIG_TYPE_STRING);
    config_setting_set_string(setting, path);

    setting = config_setting_add(us, CFG_SCOPE_SIZE, CONFIG_TYPE_STRING);
    config_setting_set_int(setting, 0);

}

rdb_err_t rdb_ctx_uscpvault(rdb_context *ctx, rdb_uuid *uuid)
{

}

/******************************************************************/

rdb_err_t load_config(config_t *(*cfg), const char *path)
{
    /* Malloc some space for us to use */
    (*cfg) = (config_t*) malloc(sizeof(config_t));

    /* Read the file. If there is an error, report it and exit. */
    if(! config_read_file(*cfg, path))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(*cfg), config_error_line(*cfg), config_error_text(*cfg));
        config_destroy(*cfg);
        free(*cfg);

        return CFG_READ_FAILED;
    }

    return SUCCESS;
}

rdb_err_t save_config(config_t *cfg, const char *path)
{

    /* Write out the new configuration. */
    if(!config_write_file(cfg, path)) {
        fprintf(stderr, "Error while writing file.\n");
        return CFG_WRITE_FAILED;
    }

    return SUCCESS;
}

rdb_err_t create_newconfig(config_t **config)
{

    /********************************************/
    /* Some data we need to populate our config */

    char *scl_t = CFG_SCL_FIXED;
    int cpus = (int) sysconf(_SC_NPROCESSORS_ONLN);
    if(cpus < 4) scl_t = CFG_SCL_FIXED;
    if(cpus >= 4 || cpus < 10) scl_t = CFG_SCL_DYNAMIC;
    if(cpus >= 10) scl_t = CFG_SCL_FIXED;

    /* Get the current time */
    time_t now = time(0);

    /********************************************/

    config_setting_t *root, *setting;
    int i;

    (*config) = (config_t*) malloc(sizeof(config_t));
    config_t *cfg = *config;

    config_init(cfg);
    root = config_root_setting(cfg);

    /* Add an empty group for scope */
    config_setting_add(root, CFG_SCOPE, CONFIG_TYPE_GROUP);

    /* Capture creation date */
    setting = config_setting_add(root, CFG_CREATION_DATE, CONFIG_TYPE_INT64);
    config_setting_set_int64(setting, now);

    /* Allocate space for last modified date */
    setting = config_setting_add(root, CFG_MODIFY_DATE, CONFIG_TYPE_INT64);
    config_setting_set_int64(setting, now);

    /* Store the current scale factor for future use */
    setting = config_setting_add(root, CFG_SCL_TYPE, CONFIG_TYPE_STRING);
    config_setting_set_string(setting, scl_t);

    setting = config_setting_add(root, CFG_SCL_FACTOR, CONFIG_TYPE_INT);
    int small_cpu = ((cpus / 2) < 0) ? 1 : (cpus / 2); // Will be at least 1
    config_setting_set_int(setting, (scl_t == CFG_SCL_FIXED) ? small_cpu : cpus);

    return SUCCESS;
}

rdb_err_t populate_paths(char *(*cfg_path), char *(*log_path), char *(*dir_path))
{
    /* Get the default scope path and attempt to load it */
    unsigned int curr_os = rdb_platform_getos();
    if(curr_os != RDB_PLATFORM_LINUX) return UNSUPPORTED_ENV;

    /* Some objects require for file handling */
    wordexp_t expantion;

    wordexp(RDB_DEF_LINUX_PATH, &expantion, 0);
    char *exp = expantion.we_wordv[0];

    (*dir_path) = malloc(sizeof(char) * REAL_STRLEN(exp));
    strcpy((*dir_path), exp);

    /* Make sure we have the directory ready */
    mkdir(exp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    /* Make some space for the CONFIG path */
    size_t len = REAL_STRLEN(exp) + strlen("config");
    char config[len];
    strcpy(config, exp);
    strcat(config, "config");

    /** Now do the same thing for the log path **/
    len = REAL_STRLEN(exp) + strlen("logs/");
    char logs[len];
    strcpy(logs, exp);
    strcat(logs, "logs/");

    /* Assign the config path */
    (*cfg_path) = (char *) malloc(sizeof(char) * len);
    strcpy(*cfg_path, config);

    /* Assign the logs path */
    (*log_path) = (char *) malloc(sizeof(char) * len);
    strcpy(*log_path, logs);

    return SUCCESS;
}
