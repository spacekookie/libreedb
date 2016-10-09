#include "crypto.h"

#include <utils/random.h>
#include <reedb/reedb.h>
#include <utils/uuid.h>
#include <defines.h>

#include <malloc.h>
#include <gcrypt.h>

#define CHECK_ENGINE if(e == NULL) return INVALID_PARAMS;

#define CHECK_TARGET(t) \
    if(t == NULL || t->key == NULL || \
            t->data == NULL || t->mode == NULL) return INVALID_TARGET;


rdb_err_t rcry_engine_init(rcry_engine *(*e), bool realtime, unsigned char *seed, size_t seed_len)
{
    rdb_err_t err;
    rcry_engine *tmp;

    /** Reserve enough memory for base engine */
    tmp = (rcry_engine*) calloc(sizeof(rcry_engine), 1);
    if(tmp == NULL) return MALLOC_FAILED;

    /** Initialise the queue */
    err = rdb_queue_init(&tmp->job_queue);
    if(err) return err;

    tmp->realtime = realtime;
    tmp->lock = false;

    size_t __seed_len = strlen((char*) seed);
    if(__seed_len != seed_len) printf("[WARN]: Non- zero-term seed!\n");

    tmp->token_seed = (unsigned char*) malloc(sizeof(unsigned char) * seed_len);
    strcpy((char*) tmp->token_seed, (char*) seed);

    (*e) = tmp;
    return SUCCESS;
}


rdb_err_t rcry_engine_seed(rcry_engine *e, size_t seed_len)
{
    CHECK_ENGINE

    rdb_err_t err = CONTEXT_LOCKED;

    /* Check lock and if free --> LOCK */
    if(e->lock) return err;
    e->lock = true;

    char *seed;
    err = rdb_rand_genset(&seed, seed_len, RDB_RAND_SECURE | RDB_RAND_SUPER_RAND);
    if(err) goto exit;

    /* Free old seed if it exists */
    if(e->token_seed) free(e->token_seed);

    e->token_seed = (unsigned char*) malloc(sizeof(unsigned char) * seed_len);
    if(e->token_seed == NULL) {
        err = MALLOC_FAILED;
        goto exit;
    }

    memcpy(e->token_seed, seed, sizeof(unsigned char) * seed_len);
    e->seed_len = seed_len;
    err = SUCCESS;

    /* Whatever we do, we MUST unlock the state */
    exit:
    e->lock = false;
    return err;
}


rdb_err_t rcry_engine_addjob(rcry_engine *e, unsigned long *id, target_t type, const unsigned char *key,
                             size_t key_len, const unsigned char *data, size_t data_len)
{
    CHECK_ENGINE

    unsigned long _id = e->id++;

    rcry_target *t = malloc(sizeof(rcry_target) * 1);

    /** Allocate memory for key mirror */
    if(REAL_STRLEN((char*) key) != key_len) return INVALID_PARAMS;
    t->key = malloc(sizeof(unsigned char) * key_len);

    /** Copy computation key into atomic struct */
    if(t->key == NULL) return MALLOC_FAILED;
    strcpy((char*) t->key, (char*) key);
    t->key_len = key_len;

    /** Allocate memory for data mirror */
    if(REAL_STRLEN((char*) data) != data_len) return INVALID_PARAMS;
    t->data = malloc(sizeof(unsigned char) * data_len);

    /** Copy computation data into atomic struct */
    if(t->data == NULL) return MALLOC_FAILED;
    strcpy((char*) t->data, (char*) data);
    t->data_len = data_len;

    /* Assign compute type */
    t->mode = type;

    // /* Generate a UUID token for the job */
    // rdb_uuid uuid;
    // rdb_uuid_create(&uuid);
    // t->token = calloc(sizeof(rdb_uuid), 1);
    // memcpy(t->token, &uuid, sizeof(rdb_uuid));

    /** Add job to engine job queue */
    rdb_queue_add(e->job_queue, t, -1);

    /* Finally assign the job ID and return */
    *id = _id;
    return SUCCESS;
}


rdb_err_t rcry_engine_nextjob(rcry_engine *e)
{
    CHECK_ENGINE

    if(e->lock) return CONTEXT_LOCKED;

    /* Retrieve the next job from the queue */
    rcry_target *t;
    rdb_queue_pop(e->job_queue, (void**) &t);

    CHECK_TARGET(t)

    printf("Job processing...done!");
}


/**
 * STATELESS FUNCTION!
 *
 *
 *
 * @param data
 * @param md
 * @param type
 * @return
 */
rdb_err_t rcry_hash_data(char *data, size_t d_len, unsigned char **md, enum rcry_hash_t type)
{
    int algo;

    switch(type) {
        case SHA256:
            algo = GCRY_MD_SHA256;
            break;

        case TIGER2:
            algo = GCRY_MD_TIGER2;
            break;

        default:
            printf("Unknown hashing algorithm! Aborting.\n");
            return HASHING_FAILED;
    }

    gcry_md_hd_t md_ctx;
    gcry_error_t gerr;

    /** Open hash context for USER cipher (default SHA256) */
    gerr = gcry_md_open(&md_ctx, algo, GCRY_MD_FLAG_SECURE);
    if(gerr) {
        printf("Failed to open digest context!\n");
        return HASHING_FAILED;
    }

    /** Add data to hash buffer either with or without provided d_len */
    if(d_len == 0)      gcry_md_write(md_ctx, data, strlen(data));
    else                gcry_md_write(md_ctx, data, d_len);

    gcry_md_final(md_ctx);

    /** Read md buffer and clear context */
    (*md) = gcry_md_read(md_ctx, PASSWD_HASH_FUNCT);
    gcry_md_close(md_ctx);

    return SUCCESS;
}