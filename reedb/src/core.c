/* reedbd-http - core.c
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

/* Internal requirements in Reedb */
#include "reedb/core.h"

/* Make sure that the error codes are all present */
#include "reedb/defs.h"

#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Permanent state variables */
static bool was_init = false;

/** Temp variables - Will be nulled after init */
static bool tmp_daemon = false;
static bool tmp_override = false;
static unsigned int tmp_passlength;
static char *tmp_path;
static ree_os tmp_os;
static int settings[6];

// static vault **active_vaults[];

int __PASSLENGTH__ 	= 0;
int __USRCODE__ 		= 1;
int __DAEMON__ 			= 2;
int __PATH__ 				= 3;
int __OS__ 					= 4;
int __OVERRIDE__ 		= 5;

ree_err_t rdb_set_passlength(unsigned int length)
{
	if (was_init)
	{
		fputs(ERR_INIT_MSG, stderr);
		return ALREADY_INIT;
	}

	if (length <= MIN_PASSLENGTH)
	{
		char error[] = "Can not set minimal passphrase length lower than %d", MIN_PASSLENGTH;
		fputs(error, stderr);
		return -1;
	}
	tmp_passlength = length;
	settings[__PASSLENGTH__] = 1;
	return 0;
}

ree_err_t rdb_set_usrcode(void *funct)
{
	if (was_init)
	{
		fputs(ERR_INIT_MSG, stderr);
		return ALREADY_INIT;
	}
	settings[__USRCODE__] = 1;
	return 0;
}

ree_err_t rdb_set_daemon(bool daemon)
{
	printf("Length: %d\n", tmp_passlength);
	if (was_init)
	{
		fputs(ERR_INIT_MSG, stderr);
		return ALREADY_INIT;
	}
	tmp_daemon = daemon;
	settings[__DAEMON__] = 1;
	return 0;
}

ree_err_t rdb_set_path(char *path)
{
	if (was_init)
	{
		fputs(ERR_INIT_MSG, stderr);
		return ALREADY_INIT;
	}

	tmp_path = path;
	settings[__PATH__] = 1;
	return 0;
}
;

ree_err_t rdb_set_os(ree_os man_os)
{
	if (was_init)
	{
		fputs(ERR_INIT_MSG, stderr);
		return ALREADY_INIT;
	}
	tmp_os = man_os;
	settings[__OS__] = 1;
	return 0;
}

ree_err_t rdb_set_override(bool override)
{
	if (was_init)
	{
		fputs(ERR_INIT_MSG, stderr);
		return ALREADY_INIT;
	}
	tmp_override = override;
	settings[__OVERRIDE__] = 1;
	return 0;
}

ree_err_t reedb_init(reedb_c *(*container))
{

	if ((*container) == NULL)
	{
		if(RDB_DEBUG) fputs("Missing container struct!\n", stderr);
		return MISSING_CONTAINER;
	}

	(*container) = calloc(sizeof(reedb_c), 1);
	if ((*container) == NULL)
	{
		if(RDB_DEBUG) fputs("Container malloc failed\n", stderr);
		return MALLOC_FAILED;
	}

	if (settings[__PASSLENGTH__])
	{
		(*container)->passlength = tmp_passlength;
	}
	else
	{
		if(RDB_DEBUG) fputs("Failed to provide password length.\n", stderr);
		return MISSING_PARAMS;
	}

	// TODO: Check that user code is present here
	if (settings[__OS__])
	{
		(*container)->os = tmp_os;
	}
	else
	{
		// TODO: PARSE OS HERE
		if(RDB_DEBUG) fputs("No OS was provided...", stderr);
		if (false)
		{
			if(RDB_DEBUG) fputs("and OS parse failed.", stderr);
			return OS_PARSE_FAILED;
		}
		printf("\n");
	}

	/* Get secondary settings here */
	if (settings[__DAEMON__])			(*container)->daemon = tmp_daemon;
	if (settings[__PATH__])				(*container)->path = tmp_path;

	//TODO: Check if a lock exists
	bool locked = false;
	if (locked && !tmp_override)
	{
		if(RDB_DEBUG) fputs("A previous instance of Reedb is still present.\n", stderr);
		return ZOMBIE_INSTANCE;
	}

	/* This is used by other modules to guarantee the existence of the core module */
	(*container)->active = true;

	/**
	 *
	 * { :key => :value, :data => {  } }
	 *
	 * active_vaults
	 * {'abc' => { name, path, data => {  } }}
	 *
	 */

	/** */
	//	(*active_vaults[]) = malloc(sizeof(*active_vaults) * 5);

	if(RDB_DEBUG) printf("Reedb seems to have successfully initialised!\n");

	/* Then set the init field to true */
	was_init = true;
	return 0;
}

ree_err_t reedb_terminate(reedb_c *(*container), char *reason)
{
	if (!was_init)
	{
		if(RDB_DEBUG)
		{
			printf(ERR_NOT_INIT, "__CORE__");
			printf("\n");
		}
		return NOT_INITIALISED;
	}

	int count;
	was_init = false;

	free((*container));
	(*container) = NULL;
	if(RDB_DEBUG) printf("Reedb is now shut down. Do not use your container pointer anymore!\n");
	return 0;
}

/** Returns if this module was successfully initialised */
bool reedb_isActive()
{
	return was_init;
}

unsigned int get_active_count()
{
	return 5;
}
