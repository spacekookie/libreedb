/* reedb - vault.c
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
 
#include <stdbool.h>
#include <stdio.h>
#include "reedb/vault.h"
#include "reedb/core.h"
#include "reedb/crypto/token.h"
#include "reedb/defs.h"

// Internals
#include "ree_vault.h"
#include "datafile.h"

static bool active = false;

/** Initialise the vault module with an existing Reedb container */
ree_err_t rdb_vault_init(reedb_c *(*container)) {
	if (!(*container)->active) {
		fputs(ERR_CORE_NOT_INIT, stderr);
		return NOT_INITIALISED;
	}

	active = true;
	return SUCCESS;
}

/*
 *  Returns a list of available vaults. Uses the same struct
 * as internal workings with the entries field nulled out.
 */
ree_err_t rdb_vault_list(rdb_vault **list) {
	return SUCCESS;
}

ree_err_t rdb_vault_scope(ree_uuid **uuid, char *name, char *path) {

}

ree_err_t rdb_vault_unscope(ree_uuid *uuid) {

}

ree_err_t rdb_vault_create(ree_token **token, ree_uuid **uuid, char *name,
		char *path, char *passphrase) {

	/** Checks if the vault already exists in the active vault set */
	int count;
//	ree_vault *active = get_active_vaults();


}

ree_err_t rdb_vault_authenticate(ree_token *token, ree_uuid *uuid) {

}

ree_err_t rdb_vault_headers(ree_uuid *uuid, ree_token *token, char *search) {

}

ree_err_t rdb_vault_file(ree_uuid *uuid, ree_token *token) {

}

ree_err_t rdb_vault_insert(ree_uuid *uuid, ree_token *token, char *data) {

}

ree_err_t rdb_vault_delete(ree_uuid *uuid, ree_token *token, char *file) {

}

ree_err_t rdb_vault_close(ree_uuid *uuid, ree_token *token) {

}

bool vault_isActive() {

}
