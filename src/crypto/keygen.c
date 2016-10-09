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

rdb_err_t rcry_keygen_aes(char **key)
{
    printf("Generating new AES256 key...\n");

    size_t key_len = rcry_keygen_sizeinfo(AES256);

    char *secure;
    rdb_rand_genset(&secure, key_len, RDB_RAND_SECURE | RDB_RAND_SUPER_RAND);

    /* Assign key and return */
    (*key) = secure;
    return SUCCESS;
}


rdb_err_t rcry_keygen_camellia(char **key)
{
    printf("Generating new AES256 key...\n");

    size_t key_len = rcry_keygen_sizeinfo(CAMELLIA256);

    char *secure;
    rdb_rand_genset(&secure, key_len, RDB_RAND_SECURE | RDB_RAND_SUPER_RAND);

    /* Assign key and return */
    (*key) = secure;
    return SUCCESS;
}


size_t rcry_keygen_sizeinfo(rcry_key_t type)
{
    switch(type) {
        case CAMELLIA256:
        case AES256:
            return 256;

        case RSA4096:
            return 4096;

        case RSA8192:
            return 8192;

        default:
            return 0;
    }
}


rdb_err_t rcry_keygen_rsa(unsigned char **pub, unsigned char **pri)
{

    return SUCCESS;
}