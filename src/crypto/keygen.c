#include "crypto.h"

// Reedb internal requirements
#include <utils/random.h>
#include <reedb/reedb.h>
#include <utils/uuid.h>
#include <defines.h>

// Environment requirements
#include <malloc.h>
#include <stdarg.h>
#include <gcrypt.h>

rdb_err_t rcry_keygen_aes(unsigned char **key)
{
    printf("Generating new AES256 key...\n");

    size_t key_len = rcry_keygen_sizeinfo(AES256);

    unsigned char *secure;
    rdb_rand_genset((char**) &secure, key_len, RDB_RAND_SECURE | RDB_RAND_SUPER_RAND);

    /* Assign key and return */
    (*key) = secure;
    return SUCCESS;
}


rdb_err_t rcry_keygen_camellia(unsigned char **key)
{
    printf("Generating new AES256 key...\n");

    size_t key_len = rcry_keygen_sizeinfo(CAMELLIA256);

    unsigned char *secure;
    rdb_rand_genset((char**) &secure, key_len, RDB_RAND_SECURE | RDB_RAND_SUPER_RAND);

    /* Assign key and return */
    (*key) = secure;
    return SUCCESS;
}


rdb_err_t rcry_keygen_rsa(unsigned char **pub, unsigned char **pri)
{

    return SUCCESS;
}