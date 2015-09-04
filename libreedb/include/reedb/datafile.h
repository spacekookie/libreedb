/*
 * datafile.h
 *
 *  Created on: 20 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_DATAFILE_H_
#define SRC_DATAFILE_H_

#include <stdbool.h>
#include "reedb/defs.h"

/** Defines the type of data that should be */
typedef enum rdb_data_t
{
	string, integer, boolean;
} rdb_data_t;

/*
 * This struct can hold generic data. It carries a type
 * and size to determine as what data type and field size
 * it should be read.
*/
typedef struct rdb_gendata
{
	rdb_data_t 	type;
	size_t			size;

	union 
	{
		int 		*iptr;
		char		*cptr;
		bool 		*bptr;
	}

} rdb_gendata;

typedef struct ree_file
{
	char 			*name,
	hmap 			*head,
	hmap 			*body,
	size_t 		body_size;

	// head 			: [ field : string ] => [ value : rdb_gendata ]
	// body 			:	Array [ HashMap [ field_name : string ] => [ rdb_gendata* ] ]
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
