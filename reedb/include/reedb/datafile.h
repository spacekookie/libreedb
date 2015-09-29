/*
 * (c) 2014 Lonely Robot.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 *
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 *
 * Author: Katharina 'spacekookie' Sabel
 *
 */

#ifndef SRC_DATAFILE_H_
#define SRC_DATAFILE_H_

#include <stdbool.h>
#include "reedb/defs.h"

/** Defines the type of data that should be */
typedef enum rdb_data_t
{
	string, integer, boolean
} rdb_data_t;

/*
 * This struct can hold generic data. It carries a type
 * and size to determine as what data type and field size
 * it should be read.
*/
typedef struct rdb_gendata
{
	rdb_data_t 	type;
	size_t			size; // > 1 means it's an array.

	union v
	{
		int 			*iptr;
		char			*cptr;
		bool 			*bptr;
	} v;

} rdb_gendata;

/** Defines the file header which is passed out to users */
typedef struct ree_file_h
{
	char			*name;
	char			*category;
	map_t			*tags;
} ree_file_h;

/** Definition of the actual datafile struct. Not exposed to user */
typedef struct ree_file
{
	/* Some metadata (again) */
	char 					*name;					// String name. Is hashed for the filesystem name
	ree_file_h		*header;				// Pointer to header struct. Used for indexing
	size_t 				body_size;			// Size of the body map (revision numbers)

	/* Actual datastorage (contains 25% more crypto than the competition) */
	map_t					*body;					// Nested body maps for revisions and data
} ree_file;

/** Creates a new file with a name */
ree_err_t rdb_create_file(ree_file **file, char *name);

/** Needs to be called before a transaction to prevent race conditions */
ree_err_t rdb_lock_file(ree_file **file);

/** Needs to be called again after a transaction */
ree_err_t rdb_unlock_file(ree_file **file);

ree_err_t rdb_insert_data(ree_file *file, rdb_gendata *version, size_t version_size);

ree_err_t rdb_delete_data(ree_file *file, rdb_gendata *field);

ree_err_t rdb_update_header(ree_file *file, ...);

ree_err_t rdb_get_header(ree_file *file);

ree_err_t rdb_read_file(ree_file *file, bool versioning);

ree_err_t sync(ree_file *file, char mode);

ree_err_t close(ree_file *file);

ree_err_t rdb_remove_file(ree_file *file);

#endif /* SRC_DATAFILE_H_ */
