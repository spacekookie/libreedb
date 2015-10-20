/* reedb - datafile.c
 *
 * (c) 2015 					Lonely Robot.
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
/* Private dependencies */
#include "datafile.h"

/* System dependencies */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 'Public' dependencies */ 
#include "reedb/utils/hashmap.h"
#include "reedb/defs.h"

static size_t START_REV_SIZE = 3;

/** Creates a new file with a name */
ree_err_t rdb_create_file(datafile **file, char *name)
{
	/* Malloc memory for the actual datafile struct */
	(*file) = malloc(sizeof(datafile));
	if((*file) == NULL)	return MALLOC_FAILED;

	/* Malloc memory for the file header struct */
	datafile_h *header = malloc(sizeof(datafile_h));
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
ree_err_t rdb_lock_file(datafile *file)
{
	return SUCCESS;
}

/** Needs to be called again after a transaction */
ree_err_t rdb_unlock_file(datafile *file)
{
	return SUCCESS;
}

ree_err_t rdb_insert_data(datafile *file, char *field, gen_data *data)
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

ree_err_t rdb_delete_data(datafile *file, char *field)
{
	int res = hashmap_remove(file->body[file->bsize], field);

	if(res == MAP_OK) return SUCCESS;
	else 							return FILE_RM_FAILED;
}

ree_err_t rdb_finalize_version(datafile *file)
{
	if(file->body[file->bsize] == NULL)
	{
		if(RDB_DEBUG) printf("Trying to finalise an empty version for %s. Aborting!", file->name);
		return FILE_EMPTY_VERSION;
	}

	/* Now just increment the revision version number*/
	file->bsize++;
	return SUCCESS;
}

ree_err_t rdb_update_header(datafile *file, ...)
{
	return SUCCESS;
}

ree_err_t rdb_get_header(datafile *file, datafile_h **headreq)
{
	/* Get our header and check for it's validity */
	datafile_h *point = file->header;

	if(	point == NULL 			|| 
			point->name == NULL || 
			point->category == NULL )	return FILE_BAD_HEADER;

	/* Then write out the pointer and return success */
	(*headreq) = point;
	return SUCCESS;
}

ree_err_t rdb_read_file(datafile *file, bool rev_a)
{
	return SUCCESS;
}

ree_err_t sync(datafile *file, char mode)
{
	if(charcmp(mode, 'q') == 0)
	{

	} else {
		if(RDB_DEBUG)
		{
			char msg[] = "Mode %c is not supported or known to Reedb!", mode;
			fputs(msg, stderr);
		}
		return FAILURE;
	}


	return SUCCESS;
}

ree_err_t close(datafile *file)
{
	return SUCCESS;
}

ree_err_t rdb_remove_file(datafile *file)
{
	return SUCCESS;
}
