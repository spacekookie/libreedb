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

/* System requirements */ 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdbool.h>
#include <stdio.h>

/* Internal requirements */
#include "reedb/crypto/token.h"
#include "reedb/vault.h"
#include "reedb/core.h"
#include "reedb/defs.h"

/* Data storage */
#include "utils/files.h"
#include "ree_vault.h"
#include "datafile.h"

/* Holds vault module state */
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

ree_err_t rdb_vault_create(ree_token *(*token), ree_uuid *(*uuid), char *name, char *path, char *passphrase)
{
	if(name == NULL)
	{
		if(RDB_DEBUG) fputs("Name provided was NULL!\n", stderr);
		return VAULT_CREATE_FAILED;
	}

	if(path == NULL)
	{
		if(RDB_DEBUG) fputs("Path provided was NULL!\n", stderr);
		return VAULT_CREATE_FAILED;
	}

	if(passphrase == NULL)
	{
		if(RDB_DEBUG) fputs("Passphrase provided was empty!\n", stderr);
		return VAULT_CREATE_FAILED;
	}

	/* Malloc a struct */
	vault *vault = malloc(sizeof(struct vault));
	if(vault == NULL)		return MALLOC_FAILED;

	/* Vault directory tree
	 * 
	 * vault
	 * ├── config.json
	 * ├── zones.json
	 * ├── keystore
	 * │  	 └── user keys
	 * ├── datastore
	 * │  	 └── data files
	 * └── parity
	 *			 └── parity checksums
	 */
	int folder;

	/** Check that the path is valid (doesn't already exist) */
	folder = mkdir(rdb_concat_path(path, name), 0755); 
	if(folder != 0)
	{
		if(RDB_DEBUG) printf("Creating the path %s failed!\n", path);
		return VAULT_CREATE_FAILED;
	}
	/** Check that the path is valid (doesn't already exist) */
	folder = mkdir("%s", 0755);
	if(folder != 0)
	{
		char msg[] = "The path provided was not empty! Code %d\n", folder;
		if(RDB_DEBUG) fputs(msg, stderr);
		return VAULT_CREATE_FAILED;
	}

	char *keystore = rdb_concat_path(2, path, "keystore");
	folder = mkdir(keystore, 0755);
	if(folder != 0) goto param_failure;
	free(keystore);

	char *datastore = rdb_concat_path(2, path, "datastore");
	folder = mkdir(datastore, 0755);
	if(folder != 0) goto param_failure;
	free(datastore);

	char *parity = rdb_concat_path(2, path, "parity");
	folder = mkdir(parity, 0755);
	if(folder != 0) goto param_failure;
	free(parity);

	/* Dump the core config */
	// JSON_dmp("{}", "%s/config.json", path);

	return SUCCESS;

	/** This label is used to GOTO it during parameter init failures */
param_failure:

	//TODO: Find a way to do this with fputs (I want it on stderr, not stdout)!
	printf("Invalid parameters --- name: %s, path: %s, passphrase: %s ---", &name, &path, &passphrase);
	return VAULT_CREATE_FAILED;
}

ree_err_t rdb_vault_authenticate(ree_token *token, ree_uuid *uuid)
{

}

ree_err_t rdb_vault_headers(ree_uuid *uuid, ree_token *token, char *search)
{

}

ree_err_t rdb_vault_file(ree_uuid *uuid, ree_token *token)
{

}

ree_err_t rdb_vault_insert(ree_uuid *uuid, ree_token *token, char *data)
{

}

ree_err_t rdb_vault_delete(ree_uuid *uuid, ree_token *token, char *file)
{

}

ree_err_t rdb_vault_close(ree_uuid *uuid, ree_token *token)
{

}

bool vault_isActive()
{

}
