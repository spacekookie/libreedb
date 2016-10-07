/* libreedb - datafile.h
 *
 * TODO: Fill in this description
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

#include <reedb/errors.h>
#include <reedb/data.h>

/* Replacement for rdb_data */
#include <dtree/dtree.h>

#include <stdbool.h>
#include <list.h>

#ifndef REEDB_DATAFILE_H
#define REEDB_DATAFILE_H

typedef enum cache_mode {
    /* Drop from cache after every action */
    HOTDROP,

    /* Keep in cache for limited time (defined in config) */
    TIMEOUT,

    /* Keep in cache until daemon shuts down */
    ENDLESS
} cache_mode;

typedef struct record {
    long            type;
    char            *name;
    List            *categories, *tags;

    unsigned int    size;
    unsigned int    version;
    bool            locked;

    /* Union data store*/
    union {
        dtree       *root;
        dtree       **list;
    } data;

    /* Additional pointers for transfer caching */
    char            *encode;
    char            *cry_encode;
} record;

rdb_err_t rdb_df_createnew(record *rec);

#endif //REEDB_DATAFILE_H
