/*
 * (c) 2015 Lonely Robot.
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

/* Include the definitions */
#include "reedb/datafile.h"

/* Things we need to run properly */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "reedb/utils/hashmap.h"
#include "reedb/defs.h"

static size_t START_REV_SIZE = 3;

/** Creates a new file with a name */
ree_err_t rdb_create_file(ree_file **file, char *name)
{
	/* Malloc memory for the actual ree_file struct */
	(*file) = malloc(sizeof(ree_file));
	if((*file) == NULL)	return MALLOC_FAILED;

	/* Malloc memory for the file header struct */
	ree_file_h *header = malloc(sizeof(ree_file_h));
	if(header == NULL)	return MALLOC_FAILED;

	/* Malloc memory for the revisions array */
	map_t *revisions = calloc(START_REV_SIZE, sizeof(map_t*));
	if(revisions == NULL) return MALLOC_FAILED;

	/* Fill in the data we already have for the file */
	(*file)->name = name;
	(*file)->header = header;
	(*file)->body = &revisions;
	(*file)->bsize = 0;

	/* Return SUCCESS because everything went swell :) */
	return SUCCESS;
}

/** Needs to be called before a transaction to prevent race conditions */
ree_err_t rdb_lock_file(ree_file *file)
{
	return SUCCESS;
}

/** Needs to be called again after a transaction */
ree_err_t rdb_unlock_file(ree_file *file)
{
	return SUCCESS;
}

ree_err_t rdb_insert_data(ree_file *file, char *field, rdb_gendata *data)
{
	/** Check if we need to increase our revision space */
	if(file->bsize >= file->rsize)
	{
		file->rsize *= 2;
		map_t *new_revs = malloc(sizeof(map_t*) * file->rsize);

		/* Check again that the malloc was successful */
		if(new_revs == NULL) return MALLOC_FAILED;

		/** Copy over data, free old array and replace the pointer in the struct */
		memcpy(file->body, new_revs, file->bsize);
		free(file->body);
		file->body = new_revs;
	}

	/* Do the hashmap insertion */
	int res = hashmap_put(file->body[file->bsize], field, data);

	/* Return our success */
	if(res == MAP_OK) return SUCCESS;

	/* Otherwise return an insertion failure */
	else return FILE_INSERT_FAILED;
}

ree_err_t rdb_delete_data(ree_file *file, char *field)
{
	int res = hashmap_remove(file->body[file->bsize], field);

	if(res == MAP_OK) return SUCCESS;
	else 							return FILE_RM_FAILED;
}

ree_err_t rdb_finalize_version(ree_file *file)
{
	/* Increment the revision version number*/
	file->bsize++;

	/* Do we need to make more changes here? */
	return SUCCESS;
}

ree_err_t rdb_update_header(ree_file *file, ...)
{
	return SUCCESS;
}

ree_err_t rdb_get_header(ree_file *file)
{
	return SUCCESS;
}

ree_err_t rdb_read_file(ree_file *file, bool versioning)
{
	return SUCCESS;
}

ree_err_t sync(ree_file *file, char mode)
{
	return SUCCESS;
}

ree_err_t close(ree_file *file)
{
	return SUCCESS;
}

ree_err_t rdb_remove_file(ree_file *file)
{
	return SUCCESS;
}
