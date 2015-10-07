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

#ifndef SRC_REE_VAULT_H_
#define SRC_REE_VAULT_H_

// System imports
#include <stdbool.h>

// Internal imports
#include "utils/hashmap.h"
#include "crypto/token.h"
#include "datafile.h"
#include "defs.h"

typedef struct ree_vault
{
	/* Some metadata for the vault */
	char				id[32]; 								// UUID string
	size_t			size;										// number of files
	char				*name;									// User defined name
	char 				*path;									// Path on FS

	time_t			created;								// Date created
	time_t			modified;								// Date last modified

	/* Some crypto nonsense :) */
	map_t				*keystore;							// Map of files -> crypt-keys or regions -> crypt-keys
	map_t 			*tokens;								// Allowed access tokens

	/* Maps for data storage */
	map_t				*tags;									// Tags ordered by category (key)
	map_t				*files;									// Datafiles ordered by their name (key)

} ree_vault;

ree_err_t rdb_create_vault(char *name, char *path);

ree_err_t rdb_get_headers();

#endif /* SRC_REE_VAULT_H_ */
