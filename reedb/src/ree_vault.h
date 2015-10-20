/* reedb - ree_vault.h
 * 
 * This file is the entry point to the Reedb HTTP(s) extention.
 * It is statically compiled against Reedb and adds functions for
 * web applications and usability without using the C bound API.
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

#ifndef SRC_REE_VAULT_H_
#define SRC_REE_VAULT_H_

// System imports
#include <stdbool.h>

// Internal imports
#include "reedb/utils/hashmap.h"
#include "reedb/crypto/token.h"
#include "reedb/defs.h"

typedef struct vault
{
	/* Some metadata for the vault */
	char				id[32]; 					// UUID string
	size_t			size;							// number of files
	char				*name;						// User defined name
	char 				*path;						// Path on FS

	time_t			created;					// Date created
	time_t			modified;					// Date last modified

	/* Some crypto nonsense :) */
	map_t				*keystore;				// Map of files -> crypt-keys or regions -> crypt-keys
	map_t 			*tokens;					// Allowed access tokens

	/* Maps for data storage */
	map_t				*tags;						// Tags ordered by category (key)
	map_t				*files;						// Datafiles ordered by their name (key)
} vault;

ree_err_t rdb_create_vault(char *name, char *path);

ree_err_t rdb_unlock_vault(vault *vault, char *passphrase, ...);

ree_err_t rdb_get_headers(vault *vault);

#endif /* SRC_REE_VAULT_H_ */
