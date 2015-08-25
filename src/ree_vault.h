/*
 * ree_vault.h
 *
 *  Created on: 25 Aug 2015
 *      Author: spacekookie
 */

#include <stdbool.h>
#include "crypto/token.h"
#include "datafile.h"
#include "defs.h"

#ifndef SRC_REE_VAULT_H_
#define SRC_REE_VAULT_H_

/**
 * Container for a vault. Has a UUID, name and path for file ops,
 * entry count and a list of entries as well as some lists (heaps) for
 * header indexing.
 */
typedef struct {
	ree_uuid *uuid;
	char *name, *path;
	size_t entry_count;
	struct ree_datafile **entries;
} ree_vault;

ree_err_t rdb_create_vault(char *name, char *path);

#endif /* SRC_REE_VAULT_H_ */
