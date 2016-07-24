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

// For *some* bizzare reason this needs to go on the outside o.o
// #include "uuid.h"

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#define RDB_DEBUG true



/** Some constants to use everywhere */
static const unsigned int MIN_PASSLENGTH = 4;

static const char *ERR_CORE_NOT_INIT =
"Core module not initiated: Vault can not operate without core. Aborting!\n";

static const char *ERR_INIT_MSG =
"Can't change this parameter! Reedb was already initialised.\n";

static const char *ERR_NOT_INIT =
"Reedb module %s wasn't previously initialised\0";

#endif /* SRC_UTILS_H_ */
