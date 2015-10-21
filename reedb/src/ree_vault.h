/* reedb - ree_vault.h
 * 
 * This file contains functions that actually run on an internal
 * vault struct. They are partially called by the vaults interface
 * and partially by smaller files such as the crypto engine.
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
#include "datafile.h"

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

/* Create a new vault (DEPRECIATED)! */
ree_err_t rdb_create_vault(vault *(*vault), ree_uuid *(*uuid), char *name, char *path, char *passphrase);

/* Unlock a vault and get a token */
ree_err_t rdb_unlock_vault(vault *vault, char *passphrase, ree_token *(*token));

/* Lock a vault again */
ree_err_t rdb_lock_vault(vault *vault, ree_token *token);

/* Dump a vault from memory (more than just lock it!) */
ree_err_t rdb_dump_vault(vault *next, vault *vault);

/* Get the headers off a vault */
ree_err_t rdb_get_headers(vault *vault);

/* The search request is just handed through from the interface */
ree_err_t rdb_search_headers(vault *vault, char *search);

#endif
