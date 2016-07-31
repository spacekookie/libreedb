/* libreedb - uuid.h
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

#ifndef REEDB_UUID_H
#define REEDB_UUID_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

#include <reedb/errors.h>
#include <reedb/reedb.h>

/**
 * Allocates a new UUID object. Will need to be freed at a later time
 *
 * @param uuid
 * @return
 */
rdb_err_t rdb_uuid_alloc(rdb_uuid *(*uuid));

/**
 * Creates a new uuid object for stack usage. Memory safe!
 *
 * @param uuid
 * @return
 */
rdb_err_t rdb_uuid_create(rdb_uuid *uuid);

/**
 * Will take the contents of a rdb_uuid object and allocate them in a
 * string that's easy to use.
 *
 * @param uuid
 * @param string
 * @param size
 * @return
 */
rdb_err_t rdb_uuid_tostring(rdb_uuid *uuid, char *(*string), size_t *size);


/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
}
#endif
#endif //REEDB_UUID_H
