#include <malloc.h>
#include <reedb/reedb.h>
#include <utils/uuid.h>
#include <defines.h>
#include <utils/random.h>

#include "crypto.h"

#define CHECK_ENGINE if(e == NULL) return INVALID_PARAMS;

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

    if(REAL_STRLEN((char*) seed) != seed_len) return INVALID_PAYLOAD;
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
