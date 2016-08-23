/* libreedb - wstore.h
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

#ifndef REEDB_WSTORE_H
#define REEDB_WSTORE_H

typedef struct dir_store {
    char    *perm, *own;
} dir_store;

typedef struct blk_store {
    long    start, stop;
} blk_store;

typedef struct store_backend {
    long        type;
    char        *full_p;

    union {
        dir_store   dir;
        blk_store   blk;
    } impl;

} store_backend;

rdb_err_t rdb_store_init(store_backend *ctx, long type);

rdb_err_t rdb_store_free(store_backend *ctx)

#endif //REEDB_WSTORE_H
