/*
 * datafile.h
 *
 *  Created on: 20 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_DATAFILE_H_
#define SRC_DATAFILE_H_

#include <stdbool.h>
#include "vault.h"

typedef struct ree_datanode ree_datanode;
struct rdb_datanode
{
	char **name;
	ree_datanode *child;
	void *field;
};

typedef struct ree_datafile ree_datafile;
struct ree_datafile
{
	struct ree_vault *vault;
	char **name;
	unsigned int version;
	ree_datanode *head, *body;
};

typedef enum ree_imode_t
{
	HARD, SOFT
} ree_imode_t;

/** Creates a new reedb datafile */
ree_err_t rdb_file_create(ree_datafile **file, char *name,
		struct ree_vault *vault);

ree_err_t rdb_file_insert(ree_datafile **file, ree_datanode *node,
		ree_imode_t mode);

ree_err_t rdb_file_sync(ree_datafile *file, ree_imode_t mode);

ree_err_t rdb_file_close(ree_datafile *file);

#endif /* SRC_DATAFILE_H_ */
