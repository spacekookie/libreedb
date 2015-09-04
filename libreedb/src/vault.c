/*
 * vault.c
 *
 *  Created on: 20 Aug 2015
 *      Author: spacekookie
 */

#include <stdbool.h>
#include <stdio.h>
#include "vault.h"
#include "core.h"
#include "datafile.h"
#include "crypto/token.h"
#include "defs.h"
#include "ree_vault.h"

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
ree_err_t rdb_vault_list(ree_vault **list) {
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
