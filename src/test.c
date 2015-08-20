/*
 * test.c
 *
 *  Created on: 5 Aug 2015
 *      Author: spacekookie
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "core.h"
#include "utils.h"

int main(void)
{
	ree_err_t rtn;

	struct reedb_c *container;

	rtn = rdb_set_passlength(16);
	rtn = rdb_set_verbose(true);
	rtn = rdb_set_path("/etc/reedb/");

	rtn = reedb_init(&container);
	if (rtn != BIG_SUCCESS)
	{
		fputs("An error occurred when initialising Reedb!\n", stderr);
	}
	else
	{
		printf("Reedb was initialised successfully!\n");
	}

	free(container);
	printf("The reedb container was now successfully freed \n");
	return 0;
}
