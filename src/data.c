#include <reedb/data.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdbool.h>

#define RDB_REC_DEF_SIZE    2
#define RDB_REC_MULTIPLY    2

rdb_err_t rdb_data_malloc(rdb_data *(*data))
{
    (*data) = (rdb_data*) malloc(sizeof(rdb_data));
    if(*data == NULL) {
        printf("Creating rdb_data object FAILED");
        return MALLOC_FAILED;
    }

    memset(*data, 0, sizeof(rdb_data));

    (*data)->type = UNSET;
    return SUCCESS;
}

rdb_err_t rdb_data_resettype(rdb_data *data)
{
    if(data->type == LITERAL) {
        if(data->payload.literal) free(data->payload.literal);
    } else if(data->type == RECURSIVE) {

        /* Iterate over all children and clear them */
        int i;
        rdb_err_t err;
        for(i = 0; i < data->size; i++) {
            err = rdb_data_free(data->payload.recursive[i]);
            if(err) return err;
        }
    }

    /* Set the data type to unset */
    data->type = UNSET;
    data->size = 0;
    data->used = 0;

    return SUCCESS;
}

rdb_err_t rdb_data_addliteral(rdb_data *data, const char *literal, size_t length)
{
    /* Make sure we are a literal or unset data object */
    if(data->type != UNSET)
        if(data->type != LITERAL) return INVALID_PAYLOAD;

    /* Get rid of previous data */
    if(data->payload.literal) free(data->payload.literal);

    /* Allocate space for the data */
    char *tmp = (char *) malloc(sizeof(char) * length);
    if(tmp == NULL) {
        printf("Allocating space for literal data FAILED");
        return MALLOC_FAILED;
    }

    /* Copy the string over and store it in the union */
    strcpy(tmp, literal);
    data->payload.literal = tmp;
    data->type = LITERAL;
    data->size = length;
    data->used = length;

    return SUCCESS;
}

rdb_err_t rdb_data_addnumeral(rdb_data *data, int numeral)
{
    /* Make sure we are a literal or unset data object */
    if(data->type != UNSET)
        if(data->type != NUMERAL) return INVALID_PAYLOAD;

    data->payload.numeral = numeral;
    data->type = NUMERAL;
    data->size = sizeof(int);
    data->used = sizeof(int);
    return SUCCESS;
}

rdb_err_t rdb_data_mallocrecursive(rdb_data *data, rdb_data *(*new_data))
{
    /* Make sure we are a literal or unset data object */
    if(data->type != UNSET)
        if(data->type != RECURSIVE) return INVALID_PAYLOAD;

    rdb_err_t err;

    /* This means elements already exist */
    if(data->size > 0) {

        /* Used should never > size */
        if(data->used >= data->size) {
            data->size += RDB_REC_MULTIPLY;

            // TODO Use Realloc
            rdb_data **tmp = (rdb_data**) malloc(sizeof(rdb_data*) * data->size);
            memcpy(tmp, data->payload.recursive, sizeof(rdb_data*) * data->used);

            /* Free the list WITHOUT the children! */
            free(data->payload.recursive);
            data->payload.recursive = tmp;
        }

    /* This means the data object is new */
    } else {
        rdb_data **tmp = (rdb_data**) malloc(sizeof(rdb_data*) * data->size);
        data->payload.recursive = tmp;
        data->type = RECURSIVE;
        data->used = 0;
        data->size = RDB_REC_DEF_SIZE;
    }

    err = rdb_data_malloc(new_data);
    if(err) return err;

    /* Reference the slot, assign it, then move our ctr */
    data->payload.recursive[data->used] = *new_data;
    data->used++;

    return SUCCESS;
}

rdb_err_t rdb_data_mallocpair(rdb_data *data, rdb_data *(*key), rdb_data *(*value))
{
    /* Make sure we are a literal or unset data object */
    if(data->type != UNSET) return INVALID_PAYLOAD;

    rdb_err_t err;

    /* Malloc two nodes */
    err = rdb_data_malloc(key);
    if(err) goto cleanup;

    err = rdb_data_malloc(value);
    if(err) goto cleanup;

    /** Malloc space for PAIR */
    data->size = 2;
    rdb_data **tmp = (rdb_data**) malloc(sizeof(rdb_data*) * data->size);
    if(!tmp) goto cleanup;

    data->payload.recursive = tmp;

    { /* Assign data to new array */
        data->payload.recursive[data->used] = *key;
        data->used++;
        data->payload.recursive[data->used] = *value;
        data->used++;
    }

    /* Assign our new type and return */
    data->type = PAIR;
    return SUCCESS;

    /* Code we run when we can't allocate structs anymore */
    cleanup:
    free(*key);
    free(*value);
    free(tmp);
    return MALLOC_FAILED;
}

rdb_err_t rdb_data_free(rdb_data *data)
{
    if(data->type == LITERAL) {
        if(data->payload.literal) free(data->payload.literal);
    } else if(data->type == RECURSIVE) {
        int i;
        rdb_err_t err;
        for(i = 0; i < data->size; i++) {
            err = rdb_data_free(data->payload.recursive[i]);
            if(err) return err;
        }

        free(data->payload.recursive);
    }

    free(data);
    return SUCCESS;
}

const char *rdb_data_dtype(rdb_uni_t type)
{
    switch(type) {
        case LITERAL: return "Literal";
        case NUMERAL: return "Numeral";
        case RECURSIVE: return "Recursive";
        default: return "Unknown";
    }
}

/**************** PRIVATE UTILITY FUNCTIONS ******************/


/**
 * Small utility function that checks if a datablock is valid to write into.
 * Potentially releases previously owned memory to prevent memory leaks
 *
 * @param data The rdb_data object to check
 * @return
 */
rdb_err_t data_check(rdb_data *data)
{
    /* Check if the data block has children */
    if(data->type == RECURSIVE)
    {
        printf("Won't override heap payload with data!");
        return INVALID_PAYLOAD;
    }

    /* Free the existing string */
    if(data->type == LITERAL)
    {
        if(data->payload.literal) free(data->payload.literal);
    }

    return SUCCESS;
}
