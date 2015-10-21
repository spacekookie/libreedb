/* reedb - vault.h
 *
 * This header file contains handles to interact with the Reedb vaults module.
 * It is dependant on information from the core and uses some utility functions from
 * hidden modules in the background. The vaults module handles all file insertion
 * and deletion operations on Reevaults.
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

#ifndef SRC_VAULT_H_
#define SRC_VAULT_H_

#include <stdbool.h>
#include "core.h"
#include "crypto/token.h"
#include "defs.h"

/* Initialise the vault module with an existing Reedb container */
ree_err_t rdb_vault_init(reedb_c *(*container));

/* Returns a list of available vaults. Uses the same struct as internal workings with the entries field nulled out. */
ree_err_t rdb_vault_list(rdb_vault **list);

ree_err_t rdb_vault_scope(ree_uuid **uuid, char *name, char *path);

ree_err_t rdb_vault_unscope(ree_uuid *uuid);

ree_err_t rdb_vault_create(ree_token **token, ree_uuid **uuid, char *name, char *path, char *passphrase);

ree_err_t rdb_vault_authenticate(ree_token *token, ree_uuid *uuid);

ree_err_t rdb_vault_headers(ree_uuid *uuid, ree_token *token, char *search);

// ree_err_t rdb_vault_headers(ree_uuid *uuid, ree_token *token);

ree_err_t rdb_vault_file(ree_uuid *uuid, ree_token *token);

ree_err_t rdb_vault_insert(ree_uuid *uuid, ree_token *token, char *data);

ree_err_t rdb_vault_delete(ree_uuid *uuid, ree_token *token, char *file);

ree_err_t rdb_vault_close(ree_uuid *uuid, ree_token *token);

bool vault_isActive();

#endif /* SRC_VAULT_H_ */
