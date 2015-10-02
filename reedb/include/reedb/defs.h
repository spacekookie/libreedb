/*
 * (C) Copyright 2014-2015 Lonely Robot.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 *
 * parameters.h
 *
 *  Created on: 6 Aug 2015
 *      Author: spacekookie
 */

#include "uuid.h"

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

/** enum error codes used throughout Reedb **/
typedef enum ree_err_t
{
	/* BOOLEAN STATES */
	TRUE, FALSE,									// Have boolean values in a standard function return

	/* General purpose error codes */
	FAILURE = -1, 								// To be used when cause of error not known.
	SUCCESS = 0,									// When something went according to plan.
	HUGE_SUCCESS = 0,							// Little Portal easter egg :)
	MISSING_PARAMS,								// No parameters were provided.
	MISSING_USRCODE,							// No user function pointer was provided. Means Reedb
																// will not do anything after being init and thus fails.
	MISSING_CONTAINER,						// Called init without providing a container struct
	MALLOC_FAILED,								// Is returned when program failed to malloc.
																// Basically means there isn't enough RAM left.
	ALREADY_INIT,									// Reedb has already been initialised in the
																// pre-defined scope.
	
	OS_PARSE_FAILED,							// Reedb failed to detect the user OS
	ZOMBIE_INSTANCE,							// 
	INVALID_PATH,									// Path provided for config is invalid.
	SHORT_PASSPHRASE,							// Passphrase violates security standards
	NOT_INITIALISED,							// Tried to use Reedb without having initialised it first

	/* Vault error returns */
	VAULT_ALREADY_SCOPED,					// Tried to scope a vault that was already in config
	VAULT_NOT_SCOPED,							// Tried to operate on vault that wasn't scope before
	VAULT_ALREADY_LOADED,					// Tried to load a vault that has already been loaded.
	VAULT_NOT_LOADED,							// Tried to operate on vault that isn't loaded yet.
	VAULT_DOESNT_EXIST,						// Tried to load vault that doesn't physically exist
																// anymore. This indicates a broken config!
	VAULT_ALREADY_EXISTS,					// Tried to create a vault that already exists on the FS.

	/* Crypto error returns */
	CRYPTO_INVALID_KEY,						// Returns when a crypto operation failed because the
																// wrong block cypher was applied.
	CRYPTO_MISSING_KEY,						// 
	CRYPTO_CORRUPT_KEY,						// 
	CRYPTO_WRONG_BLOCKSIZE,				// 
	CRYPTO_INVALID_CONTEXT,				// 
	CRYPTO_WRONG_CONTEXT,					// 
	CRYPTO_MISSING_CRYRAM,				// 
	CRYPTO_UNKNOWN_FAILURE,				// 

}ree_err_t;

/** Some constants to use everywhere */
static const unsigned int MIN_PASSLENGTH = 4;

static const char *ERR_CORE_NOT_INIT =
"Core module not initiated: Vault can not operate without core. Aborting!\n";

static const char *ERR_INIT_MSG =
"Can't change this parameter! Reedb was already initialised.\n";

static const char *ERR_NOT_INIT =
"Reedb module %s wasn't previously initialised\0";

#endif /* SRC_UTILS_H_ */
