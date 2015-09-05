/*
 * tools.c
 *
 *  Created on: 25 Aug 2015
 *      Author: spacekookie
 */

#include "defs.h"
#include "uuid.h"
#include "ree_vault.h"

void get_active_vault(ree_uuid uuid) {
	int count;

	for (count = 0; count < get_active_count(); count++) {
		// get_active_vaults()[5];
	}
}

ree_err_t search_active_vaults(char *name, char *path) {
	ree_vault *tmp = get_active_vaults();
	int count;
	for (count = 0; count < get_active_count(); count++) {
		if (tmp[count].name == name && tmp[count].path == path) {
			return VAULT_ALREADY_LOADED;
		}
	}
	return false;
}
