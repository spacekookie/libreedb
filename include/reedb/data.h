/* libreedb - data.h
 *
 * libreedb implements (and uses) many data structures provided
 * by external libraries. These data structures are sometimes now
 * easy to use and cumbersome. And they shouldn't be exposed to
 * the developers of libreedb applications and extentions.
 *
 * Thus...libreedb-data. A simple, not-threadsafe, easy to handle
 * memory structure scheme that allows you to represent any kind of
 * data inside your own application, which is then used as a carrier
 * to push changes into a libreedb vault.
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
 * Please use the API to create and destroy objects as only this way
 * memory-safety and memory leaks can be guaranteed.
 *
 * With the API you can easily create structures like the following:
 *
 * root [recursive]
 *    child1 [recursive]
 *       key [literal] - "Username"
 *       value [literal] - "spacekookie"
 *    child2 [recursive]
 *       key [literal] - "Age"
 *       value [numerical] - 23
 *    child3
 *       subchild [recursive]
 *          ...
 *
 * Freeing the root node will free all children
 */

#ifndef RDB_DATA_H
#define RDB_DATA_H

#include <memory.h>

#include <reedb/errors.h>

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UNSET, LITERAL, NUMERAL, RECURSIVE, PAIR
} rdb_uni_t;

typedef struct rdb_data {
    rdb_uni_t       type;
    size_t          size, used;
    union {
        char                *literal;
        int                 numeral;
        struct rdb_data     *(*recursive);
    } payload;
} rdb_data;

/** Malloc a new rdb_data object */
rdb_err_t rdb_data_malloc(rdb_data *(*data));

rdb_err_t rdb_data_resettype(rdb_data *data);

/** Set the data element to a literal and save it's length */
rdb_err_t rdb_data_addliteral(rdb_data *data, const char *literal, size_t length);

/** Set the data element to a numeral */
rdb_err_t rdb_data_addnumeral(rdb_data *data, int numeral);

/** Add two new elements as a PAIR node under an existing node */
rdb_err_t rdb_data_mallocpair(rdb_data *data, rdb_data *(*key), rdb_data *(*value));

/** Add a new data element to the resursive data store */
rdb_err_t rdb_data_mallocrecursive(rdb_data *data, rdb_data *(*new_data));

const char *rdb_data_dtype(rdb_uni_t type);

/** Will free all memory allocated by this element and it's children */
rdb_err_t rdb_data_free(rdb_data *data);

#ifdef __cplusplus
}
#endif
#endif //REEDB_DATA_H
