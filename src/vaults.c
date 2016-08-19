#include <reedb/reedb.h>

#include <hashtable.h>
#include <stdlib.h>
#include <stdio.h>
#include <list.h>

#include "utils/uuid.h"
#include "config.h"

#define VAULT_SANE \
    if(vault == NULL) return INVALID_VAULT; \
    if(vault->name == NULL) return INVALID_VAULT; \
    if(vault->path == NULL) return INVALID_VAULT; \
    if(vault->combined == NULL) return INVALID_VAULT; \

#define ASSIGN(field, fl) \
    if(flags & (fl)) field = fl;

#define FIELD_CHECK(field) \
    if(field == 0) { \
        printf("Failed to provide value for field %s\n", #field); \
        return INVALID_PARAMS; \
    }

struct ree_vault {
    unsigned long       type;
    unsigned long       cache;
    unsigned long       write_m;
    unsigned long       scale_t;
    unsigned long       log_t;
    unsigned long       rqlsyn_t;
    unsigned long       ftr_single, ftr_multi, ftr_tree;
    unsigned long       ftr_lut_dis, ftr_rql_dis, ftr_head_dis;
    unsigned long       ftr_perm;

    /* Vault contents */
    Hashtable           *headers, *files;
    List                *tags, *categories;
    struct threadpool   *thpool;
};

/**********************************************************************/

rdb_err_t check_flagmask(unsigned long flags, unsigned long mask);

rdb_err_t rdb_vlts_setflags(rdb_vault *vault, unsigned long flags)
{
    VAULT_SANE

    /** Allocate inner scope and clean memory */
    vault->inner = (ree_vault*) malloc(sizeof(ree_vault));
    if(vault->inner == NULL) return MALLOC_FAILED;
    memset(vault->inner, 0, sizeof(ree_vault));

    rdb_err_t err;

    /**
     *
     * This function is a bit insane. Three MACROs are invoked over and over again and this should
     * quickly explain what they do for future contributers.
     *
     * SINGLE_BIT checks if a number is power of 2 (aka only has 1 bit set) in a range. The range is
     *      determined by OR-ing all valid flags together. Thus we make sure we don't have conflicts.
     *
     * ASSIGNs (badum-tss) a field in the inner vault to a flag (if that flag is set)
     *      No further checking is done by this MACRO.
     *
     * FIELD_CHECK makes sure that the value in the field isn't 0 therefore making sure that a flag was
     *      actually provided in the end.
     *
     * The order the fields are assigned at makes a difference in case of errors during the single bit
     * checking or future bugs. It makes sure that certain values override others (e.g. verbose logging
     * overrides silent logging)
     *
     **/

    /****** Make sure the VAULT TYPE field is unique ******/
    err = check_flagmask(flags, RDB_FLG_ROOT | RDB_FLG_MULTI_USER | RDB_FLG_MANAGED | RDB_FLG_ANARCHY);
    if(err) {
        printf("Failed to provide a vault type!\n");
        return err;
    }

    /** Set fields with precidence order **/
    ASSIGN(vault->inner->type, RDB_FLG_ROOT);
    ASSIGN(vault->inner->type, RDB_FLG_MULTI_USER);
    ASSIGN(vault->inner->type, RDB_FLG_MANAGED);
    ASSIGN(vault->inner->type, RDB_FLG_ANARCHY);

    /** Check that the field is actually set **/
    FIELD_CHECK(vault->inner->type)

    /****** Make sure the CACHE type field is unique ******/
    err = check_flagmask(flags, RDB_FLG_CACHE_SECURE | RDB_FLG_CACHE_QUICK | RDB_FLG_CACHE_BUFFERED);
    if(err == 0) {

        /** Set fields with precidence order **/
        ASSIGN(vault->inner->cache, RDB_FLG_CACHE_QUICK);
        ASSIGN(vault->inner->cache, RDB_FLG_CACHE_SECURE);
        ASSIGN(vault->inner->cache, RDB_FLG_CACHE_BUFFERED);
    } else {

        vault->inner->cache = RDB_FLAG_DEFAULT_CACHING;
    }

    /** Check that the field is actually set **/
    FIELD_CHECK(vault->inner->cache)

    /****** Make sure the WRITE type field is unique ******/
    err = check_flagmask(flags, RDB_FLG_WRITE_BLOCK | RDB_FLG_WRITE_TREE);
    if(err == 0) {

        /** Set fields with precidence order **/
        ASSIGN(vault->inner->write_m, RDB_FLG_WRITE_BLOCK);
        ASSIGN(vault->inner->write_m, RDB_FLG_WRITE_TREE);
    } else {

        /* If none was provided --> set default value */
        vault->inner->write_m = RDB_FLG_DEFAULT_WRITE_MODE;
    }

    /** Check that the field is actually set **/
    FIELD_CHECK(vault->inner->write_m)

    /****** Make sure the scaling mode field is unique ******/
    err = check_flagmask(flags, RDB_FLG_SCALE_DYNAMIC | RDB_FLG_SCALE_ENDLESS | RDB_FLG_SCALE_FIXED);
    if(err == 0) {

        /** Set fields with precidence order **/
        ASSIGN(vault->inner->scale_t, RDB_FLG_SCALE_DYNAMIC);
        ASSIGN(vault->inner->scale_t, RDB_FLG_SCALE_ENDLESS);
        ASSIGN(vault->inner->scale_t, RDB_FLG_SCALE_FIXED);
    } else {

        /* If none was provided --> set default value */
        vault->inner->scale_t = RDB_FLG_DEFAULT_SCALE;
    }


    /** Check that the field is actually set **/
    FIELD_CHECK(vault->inner->scale_t)

    /****** Make sure the logging level is unique ******/
    err = check_flagmask(flags, RDB_FLG_LOG_VERBOSE | RDB_FLG_LOG_INFO |
                                RDB_FLG_LOG_WARN | RDB_FLG_LOG_ERROR | RDB_FLG_LOG_SILENT);
    if(err == 0) {

        /** Set fields with precidence order **/
        ASSIGN(vault->inner->log_t, RDB_FLG_LOG_SILENT);
        ASSIGN(vault->inner->log_t, RDB_FLG_LOG_ERROR);
        ASSIGN(vault->inner->log_t, RDB_FLG_LOG_WARN);
        ASSIGN(vault->inner->log_t, RDB_FLG_LOG_INFO);
        ASSIGN(vault->inner->log_t, RDB_FLG_LOG_VERBOSE);
    } else {

        /* If none was provided --> set default value */
        vault->inner->log_t = RDB_FLG_DEFAULT_LOGLEVEL;
    }

    FIELD_CHECK(vault->inner->log_t)


    /****** Record mode doesn't need uniqueness - thus we don't check ******/
    ASSIGN(vault->inner->ftr_single, RDB_FLG_FTR_SINGLE_RECORD)
    ASSIGN(vault->inner->ftr_multi, RDB_FLG_FTR_MULTI_RECORD)
    ASSIGN(vault->inner->ftr_tree, RDB_FLG_FTR_RECORD_TREE)


    /****** Optionally switch off some features like LUT, RQL and headers ******/
    ASSIGN(vault->inner->ftr_head_dis, RDB_FLG_FTR_DISABLE_HEADERS)
    ASSIGN(vault->inner->ftr_lut_dis, RDB_FLG_FTR_DISABLE_LUT)
    ASSIGN(vault->inner->ftr_rql_dis, RDB_FLG_FTR_DISABLE_RQL)
    ASSIGN(vault->inner->ftr_perm, RDB_FLG_FTR_PERMISSIVE)


    /****** Choose a RQL syntax version - uniqueness is key again ******/
    err =  check_flagmask(flags, RDB_FLG_RQLSYNTAX_SIMPLE | RDB_FLG_RQLSYNTAX_COMPLEX);
    if(err == 0) {

        ASSIGN(vault->inner->rqlsyn_t, RDB_FLG_RQLSYNTAX_SIMPLE)
        ASSIGN(vault->inner->rqlsyn_t, RDB_FLG_RQLSYNTAX_COMPLEX)
    } else {

        /* If none was provided --> set default value */
        vault->inner->rqlsyn_t = RDB_FLG_DEFAULT_RQLSYNTAX;
    }

    /* Return with success :) */
    return HUGE_SUCCESS;
}

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
rdb_err_t rdb_vlts_addzone(rdb_vault *vault, const char *name)
{

    return SUCCESS;
}

/**
 * Adds a new user to the vault. The name in the vault must be unique.
 *
 * @param name Unique username
 * @param zones A list of zones to add the user to
 * @return The vault user ID
 */
rdb_err_t rdb_vlts_adduser(rdb_vault *vault, const char *name, long zones)
{

    return SUCCESS;
}

/**
 * Get the user id for a user in this vault. Will return -1 if the user
 * doesn't exist
 *
 * @param username
 * @return
 */
long rdb_vlts_getuser(const char *username)
{
#define NO_SUCH_USER_RETURN -1 // Very specific for this function



    return NO_SUCH_USER_RETURN;
}

/**
 * Sets the passphrase for a specific user ID. ID 0 is by default always
 * the ROOT user. This function can only be used to do initial passphrase
 * setup
 *
 * @param user Unique user ID
 * @param passphrase
 */
rdb_err_t rdb_vlts_setlogin(rdb_vault *vault, long user, const char *passphrase)
{

    return SUCCESS;
}

/**
 * Take currently loaded settings, finalise them and finish setup. Only
 * past this point are changes to the vault final and persistent!
 *
 * This function will throw detailed errors about wrong setup parameters
 */
rdb_err_t rdb_vlts_finalise(rdb_vault *vault)
{

    return SUCCESS;
}


/*****************************************************************/

rdb_err_t check_flagmask(unsigned long flags, unsigned long mask)
{
    unsigned long x = flags & mask;
    if(x == 0) return MISSING_PARAMETERS;

    if(x != 1) {
        while(((x % 2) == 0) && x > 1) x /= 2;
        if(x != 1) return INVALID_PARAMS;
    }

    return SUCCESS;
}