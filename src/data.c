#include <reedb/data.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include <reedb/reedb.h>


#define STB_LEAKCHECK_IMPLEMENTATION
#include <stb_leakcheck.h>

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
    } else if(data->type == RECURSIVE || data->type == PAIR) {

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

rdb_err_t rdb_data_addrecursive(rdb_data *data, rdb_data *(*new_data))
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

rdb_err_t rdb_data_addpair(rdb_data *data, rdb_data *(*key), rdb_data *(*value))
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

void recursive_print(rdb_data *data, const char *offset)
{
    rdb_uni_t type = data->type;

    switch(type) {
        case UNSET:
            printf("[NULL]\n");
            break;
        case LITERAL:
            printf("%s['%s']\n", offset, data->payload.literal);
            break;
        case NUMERAL:
            printf("%s[%d]\n", offset, data->payload.numeral);
            break;
        case PAIR:
            {
                rdb_uni_t k_type = data->payload.recursive[0]->type;
                rdb_uni_t v_type = data->payload.recursive[1]->type;

                if(k_type == LITERAL) printf("%s['%s']", offset, data->payload.recursive[0]->payload.literal);
                if(k_type == NUMERAL) printf("%s[%d]", offset, data->payload.recursive[0]->payload.numeral);

                char new_offset[REAL_STRLEN(offset) + 2];
                strcpy(new_offset, offset);
                strcat(new_offset, "  ");

                if(k_type == RECURSIVE || k_type == PAIR) recursive_print(data->payload.recursive[0], new_offset);

                /* Print the value now */

                if(k_type == LITERAL) printf(" => ['%s']\n", data->payload.recursive[1]->payload.literal);
                if(k_type == NUMERAL) printf(" => [%d]\n", data->payload.recursive[1]->payload.numeral);

                if(k_type == RECURSIVE || k_type == PAIR) recursive_print(data->payload.recursive[1], new_offset);
            }
            break;

        case RECURSIVE:
        {
            int i;
            printf("%s[RECURSIVE]\n", offset);
            for(i = 0; i < data->used; i++) {
                rdb_uni_t type = data->payload.recursive[i]->type;


                char new_offset[REAL_STRLEN(offset) + 2];
                strcpy(new_offset, offset);
                strcat(new_offset, "  ");

                if(type == LITERAL || type == NUMERAL) {
                    recursive_print(data->payload.recursive[i], new_offset);
                    continue;
                }

                if(type == RECURSIVE)
                {
                    recursive_print(data->payload.recursive[i], new_offset);
                    continue;
                }

                if(type == PAIR) {
                    printf("%s[PAIR] <==> ", new_offset);
                    recursive_print(data->payload.recursive[i], new_offset);
                }
            }
            break;
        }

        default:
            break;

    }
}

void rdb_data_print(rdb_data *data)
{
    recursive_print(data, "");
}

rdb_err_t rdb_data_get(rdb_data *data, void *(*val))
{
    if(data->type == LITERAL) *val = (char*) data->payload.literal;
    if(data->type == NUMERAL) *val = (int*) &data->payload.numeral;
    if(data->type == RECURSIVE || data->type == PAIR)
        *val = (rdb_data*) data->payload.recursive;

    return SUCCESS;
}

rdb_err_t rdb_data_free(rdb_data *data)
{
    if(data == NULL) return SUCCESS;

    if(data->type == LITERAL) {
        if(data->payload.literal) free(data->payload.literal);
    } else if(data->type == RECURSIVE || data->type == PAIR) {
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
