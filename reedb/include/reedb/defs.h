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

typedef enum ree_err_t
{
	/** BOOLEAN STATES */
	TRUE,
	FALSE,

	FAILURE = -1, // To be used when cause of error not known.
	SUCCESS = 0,// When something went according to plan
	HUGE_SUCCESS = 0,// Little Portal easter egg :)
	MISSING_PARAMS = 0xA0,
	MISSING_USRCODE = 0xA1,
	MISSING_CONTAINER = 0xA2,
	MALLOC_FAILED = 0xA3,
	ALREADY_INIT = 0xA4,
	OS_PARSE_FAILED = 0xA5,
	ZOMBIE_INSTANCE = 0xA6,
	INVALID_PATH = 0xA7,
	SHORT_PASSPHRASE = 0xA8,
	NOT_INITIALISED = 0xA9,

	/* Vault error returns */
	VAULT_ALREADY_SCOPED = 0xAA,
	VAULT_NOT_SCOPED = 0xAB,
	VAULT_ALREADY_LOADED = 0xAC,
	VAULT_NOT_LOADED = 0xAD,
	VAULT_DOESNT_EXIST = 0xAF,

	/* */

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
