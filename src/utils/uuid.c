/* libreedb - uuid.c
 *
 * A collection of utility functions around UUIDs. While uuids are their own type
 * in libreedb (struct around a fixed length array), this file contains functions
 * to create them and also return their contents as a plain char string as well
 * as the length to write into other parts of the application.
 *
 * (c) 2016 					Lonely Robot.
 * Authors:						Katharina 'spacekookie' Sabel
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * -------------------------------------------
 *
 */

#include <stdio.h>

#include "uuid.h"
#include "random.h"
#include "datastore/encoding.h"

/**
 * Allocates a new UUID object. Will need to be freed at a later time
 *
 * @param uuid
 * @return
 */
rdb_err_t rdb_uuid_alloc(rdb_uuid *(*uuid))
{
    (*uuid) = (rdb_uuid*) malloc(sizeof(rdb_uuid));
    if(*uuid == NULL) return MALLOC_FAILED;

    rdb_err_t err = rdb_uuid_create(*uuid);
    if(err) {
        free(*uuid);
        return err;
    }

    return SUCCESS;
}

/**
 * Creates a new uuid object for stack usage. Memory safe!
 *
 * @param uuid
 * @return
 */
rdb_err_t rdb_uuid_create(rdb_uuid *uuid)
{
    int ret;
    rdb_err_t err;
    int cpyctr = 0;

    memset(uuid, 0, sizeof(rdb_uuid));

    /* Target string lengths */
    size_t s_blk = 6, l_blk = 8;

    /** Create some arrays to store data in **/
    char *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    unsigned char *A64, *B64, *C64, *D64;


    /**** Generate enough random data for us to generate a unique ID ****/

    err = rdb_rand_genset(&A, l_blk, RDB_RAND_NORMAL_RAND);
    if(err) goto cleanup;

    err = rdb_rand_genset(&B, l_blk, RDB_RAND_NORMAL_RAND);
    if(err) goto cleanup;

    err = rdb_rand_genset(&C, s_blk, RDB_RAND_NORMAL_RAND);
    if(err) goto cleanup;

    err = rdb_rand_genset(&D, l_blk, RDB_RAND_NORMAL_RAND);
    if(err) goto cleanup;


    /**** Encode the data in base64 for easy readability ****/

    int outLen;
    A64 = NBase58Encode((unsigned char*) A, (int) l_blk, &outLen);
    B64 = NBase58Encode((unsigned char*) B, (int) l_blk, &outLen);
    C64 = NBase58Encode((unsigned char*) C, (int) s_blk, &outLen);
    D64 = NBase58Encode((unsigned char*) D, (int) l_blk, &outLen);

    memcpy(uuid->x + cpyctr, A64, 8);
    cpyctr += 8;

    memcpy(uuid->x + cpyctr++, "-", 1);

    memcpy(uuid->x + cpyctr, B64, 8);
    cpyctr += 8;

    memcpy(uuid->x + cpyctr++, "-", 1);

    memcpy(uuid->x + cpyctr, C64, 5);
    cpyctr += 5;

    memcpy(uuid->x + cpyctr++, "-", 1);

    memcpy(uuid->x + cpyctr, D64, 8);
    cpyctr += 8;

    err = SUCCESS;

    cleanup:
    if(A) free(A);
    if(B) free(B);
    if(C) free(C);
    if(D) free(D);
    return err;
}

/**
 * Will take the contents of a rdb_uuid object and allocate them in a
 * string that's easy to use.
 *
 * @param uuid
 * @param string
 * @param size
 * @return
 */
rdb_err_t rdb_uuid_tostring(rdb_uuid *uuid, char *(*string), size_t *size)
{
    (*size) = strlen((char*) uuid->x);

    (*string) = (char*) malloc(sizeof(char) * (*size));
    strcpy((*string), (char*) uuid->x);
}

char *rdb_uuid_stringify(rdb_uuid uuid)
{
    size_t size = 33;
    if(strcmp((char*) uuid.x, "") == 0) return NULL;
    char *string = (char*) malloc(sizeof(char) * size);

    memcpy(string, (char*) uuid.x, size - 1);
    memcpy(string + size, "\0", 1);
    return string;
}