#include <reedb/random.h>

// Privatly include crypto lib
#include <gcrypt.h>

rdb_err_t rdb_rand_genset(char *(*data), size_t length, int flag)
{
    if(length <= 0) return INVALID_BUFFER_SIZE;
    rdb_err_t err = INVALID_PARAMS;

    /* Check if our input flags are valid */
    if(!(flag & (RDB_RAND_NORMAL_RAND)) && !(flag & (RDB_RAND_SUPER_RAND))
       && !(flag & (RDB_RAND_WEAK_RAND)))
            return err;

    char *buffer = NULL;

    if(flag & (RDB_RAND_SECURE)) {

        /* Only ACTUALLY use secmem if gcrypt can do it */
        if(!gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P)) {
            printf("libgcrypt hasn't been initialised for secmem yet! Falling back to non-safe memory\n");
            goto fallback;
        }

        /** Allocate one of the possible strengths of buffers in secure memory */
        if(flag & (RDB_RAND_SUPER_RAND))    (*data) = (char *) gcry_random_bytes_secure(length, GCRY_VERY_STRONG_RANDOM);
        if(flag & (RDB_RAND_NORMAL_RAND))   (*data) = (char *) gcry_random_bytes_secure(length, GCRY_STRONG_RANDOM);
        if(flag & (RDB_RAND_WEAK_RAND))     (*data) = (char *) gcry_random_bytes_secure(length, GCRY_WEAK_RANDOM);

        err = SUCCESS;

    } else {
        fallback:

        /** Allocate one of the possible strengths in normal memory */
        if(flag & (RDB_RAND_SUPER_RAND))    (*data) = (char *) gcry_random_bytes(length, GCRY_VERY_STRONG_RANDOM);
        if(flag & (RDB_RAND_NORMAL_RAND))   (*data) = (char *) gcry_random_bytes(length, GCRY_STRONG_RANDOM);
        if(flag & (RDB_RAND_WEAK_RAND))     (*data) = (char *) gcry_random_bytes(length, GCRY_WEAK_RANDOM);

        err = SUCCESS;
    }

    return err;
}

rdb_err_t rdb_rand_nonce(char *(*data), size_t length)
{
    if(length <= 0) return INVALID_BUFFER_SIZE;

    /* Allocate our buffer in memory */
    (*data) = (char*) malloc(sizeof(char) * length);

    /* Fill buffer. No errors can be returned! */
    gcry_create_nonce(*data, length);
    return SUCCESS;
}

rdb_err_t rdb_rand_gentok(char *(*data))
{

    return SUCCESS;
}
