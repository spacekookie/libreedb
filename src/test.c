/*
 * test.c
 *
 *  Created on: 5 Aug 2015
 *      Author: spacekookie
 */

#include <stdlib.h>
#include <stdio.h>
#include "core.h"
#include "vault.h"

int main(void)
{
	ree_err_t rtn;
	reedb_c *container;
	rtn = rdb_set_passlength(16);
	rtn = rdb_set_verbose(true);
	rtn = rdb_set_path("/etc/reedb/");

	rtn = reedb_init(&container);
	if (rtn != HUGE_SUCCESS)
	{
		fputs("An error occurred when initialising Reedb!\n", stderr);
	}
	else
	{
		printf("Reedb was initialised successfully!\n");
	}
	reedb_init(&container);

	/** DO STUFF WITH LIB **/
	rdb_vault_init(&container);

	ree_token 	*token;
	ree_uuid 	*uuid;

	rdb_vault_create(&token, &uuid, "default", "~/Desktop", "megapassphrase123");

	/** NO LONGER DO STUFF WITH LIB **/
	reedb_terminate(&container, "Science");

	return 0;
}
