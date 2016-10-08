#include <reedb/reedb.h>

#include "keystore.h"
#include "crypto.h"

#include <malloc.h>
#include <string.h>
#include <lzma.h>


#define CHECK_STORE if(ks == NULL) return INVALID_PARAMS;

int free_ks_item(rcry_key_c *);


rdb_err_t rcry_keystore_init(rcry_keystore *ks, const char *path)
{
    CHECK_STORE

    /* Make sure memory is clean */
    memset(ks, 0, sizeof(rcry_keystore));

    /* Create backing table */
    ks->keystore = hashmap_new();

    /* Copy keystore path into place */
    size_t path_len = REAL_STRLEN(path);
    ks->store_path = malloc(sizeof(char) * path_len);
    if(ks->store_path == NULL) return MALLOC_FAILED;
    strcpy(ks->store_path, path);

    return SUCCESS;
}


rdb_err_t rcry_keystore_add(rcry_keystore *ks, char *id, char *key, enum rcry_store_t type)
{
    CHECK_STORE

    void *ret;
    int stat = hashmap_get(ks->keystore, id, &ret);

    rcry_key_c *c;

    /* If no element was found, we need to create a new one */
    if(stat == NOT_FOUND && ret == NULL) {

        c = malloc(sizeof(rcry_key_c) * 1);
        if(c == NULL) return MALLOC_FAILED;
        memset(c, 0, sizeof(rcry_key_c));

        /* For code re-use later */
        ret = (void*) c;
    }

    c = (rcry_key_c*) ret;
    size_t size = sizeof(char) * REAL_STRLEN(key);

    /** Define a macro that does all the work locally */
#define FILL_KEY(location) \
    location = (char*) calloc(size, 1); \
    if(location == NULL) return MALLOC_FAILED; \
    memcpy(location, key, size); \
    break;

    /** Then neatly switch over it with no bloat */
    switch (type) {
        case PRIMARY:   FILL_KEY(c->primary);
        case SECONDARY: FILL_KEY(c->secondary);
        case PRIVATE:   FILL_KEY(c->pri);
        case PUBLIC:    FILL_KEY(c->pub);

        case __ALL__:   printf("Don't use __ALL__ to add a key!\n");
        default:        return INVALID_PARAMS;
    }

    return SUCCESS;
}


rdb_err_t rcry_keystore_free(rcry_keystore *ks)
{
    CHECK_STORE

    /* Free path string */
    free(ks->store_path);

    /* Free backing storage */
    hashmap_iterate(ks->keystore, (int (*)(void*)) free_ks_item);
    hashmap_free(ks->keystore);

    /* Free ks itself */
    free(ks);

    return SUCCESS;
}


/**
 * Utility function that frees a keystore item and all the data it holds
 * @param c
 */
int free_ks_item(rcry_key_c *c)
{
    /* Free key contents */
    free(c->pri);
    free(c->pub);
    free(c->primary);
    free(c->secondary);

    /* Free itself */
    free(c);

    return SUCCESS;
}
