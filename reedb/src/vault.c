/* reedb - vault.c
 * 
 * Implementation of the vault interface.
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
#include <stdbool.h>
#include <stdio.h>

/* Internal requirements */
#include "reedb/utils/hashmap.h"
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

/* Holds a static instance of a container for *reasons* */
static struct reedb_c *container;

/* Information about active vaults */
static map_t *vaults = NULL;

/** Initialise the vault module with an existing Reedb container */
ree_err_t rdb_vault_init(reedb_c *(*cont)) {
	if (!(*cont)->active) {
		fputs(ERR_CORE_NOT_INIT, stderr);
		return NOT_INITIALISED;
	}

	if(!active)
	{
	container = (*cont);
	vaults = hashmap_new();

	active = true;
	return SUCCESS;
	} else {
		if(RDB_DEBUG) fputs("Module is already init!", stderr);
		return FAILURE;
	}
}

ree_err_t rdb_vault_terminate(unsigned int mode)
{
	if(active)
	{
		vault *start;
		hashmap_get_one(vaults, start, 0);
		
		/** 
		 * Will be called for every vaults in the vaults interface. The function
		 * locks out all access to the vault, finishes all transactions in
		 * progress and then dumps the datastructure from memory.
		 */
		hashmap_iterate(vaults, rdb_dump_vault, start);

		/* Then free the actual hashmap */
		hashmap_free(vaults);
		return SUCCESS;

	} else {
		return FAILURE;
	}
}

/*
 *  Returns a list of available vaults. Uses the same struct
 * as internal workings with the entries field nulled out.
 */
ree_err_t rdb_vault_list(rdb_vault **list) {
	return SUCCESS;
}

ree_err_t rdb_vault_scope(char **uuid, char *name, char *path) {

}

ree_err_t rdb_vault_unscope(char *uuid) {

}

ree_err_t rdb_vault_create(ree_token *(*token), char *(*uuid), char *name, char *path, char *passphrase)
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

	//TODO: Change TYPE from config
	int uuidsucess = rdb_generate_uuid((*uuid), TYPE1);
	if(uuidsucess != 0)
	{
		printf("UUID Create returned %d\n", uuidsucess);
		return VAULT_CREATE_FAILED;
	}

	int toksuccess = rdb_tokens_create((*token), 0);
	if(toksuccess != 0)
	{
		printf("Token Create returned %d\n", toksuccess);
		return VAULT_CREATE_FAILED;
	}

	/* Handle the public vault struct */
	rdb_vault *pub_vault = malloc(sizeof(rdb_vault));
	pub_vault->name = name;
	pub_vault->path = path;
	pub_vault->size = 0;

	/* Put it into the hashmap */
	rdb_vault *ch_vault;
	hashmap_get(container->scoped, name, ch_vault);

	if(ch_vault != NULL)
	{
		if(ch_vault->name == pub_vault->name && ch_vault->path == pub_vault->path)
		{
			if(RDB_DEBUG) fputs("A vault by that ID is already scoped!\n", stderr);
			free(ch_vault);
			return VAULT_ALREADY_SCOPED;
		}
	} else {
		hashmap_put(container->scoped, name, pub_vault);
	}

	/* Now actually create that internal vault struct */
	vault *vault;
	int success = rdb_create_vault(&vault, (*uuid), name, path, passphrase);

	/* Store it in the static hashmap */
	ch_vault = NULL;
	hashmap_get(vaults, name, ch_vault);

	if(ch_vault->name == vault->name && ch_vault->path == vault->path)
	{
		if(RDB_DEBUG) fputs("A vault by that ID is already scoped!\n", stderr);
		free(ch_vault);
		return VAULT_ALREADY_SCOPED;
	}

	hashmap_put(vaults, name, vault);
	return success;
}

ree_err_t rdb_vault_authenticate(ree_token *token, char *uuid)
{

}

ree_err_t rdb_vault_headers(char *uuid, ree_token *token, char *search)
{

}

ree_err_t rdb_vault_file(char *uuid, ree_token *token)
{

}

ree_err_t rdb_vault_insert(char *uuid, ree_token *token, char *data)
{

}

ree_err_t rdb_vault_delete(char *uuid, ree_token *token, char *file)
{

}

ree_err_t rdb_vault_close(char *uuid, ree_token *token)
{

}

bool rdb_vault_isActive()
{
	return active;
}
