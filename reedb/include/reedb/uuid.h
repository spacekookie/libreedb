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
 * This file defines the UUID scheme used in all of Reedb
 *
 */

#ifndef SRC_UUID_H_
#define SRC_UUID_H_

#include "defs.h"

typedef enum ree_uuid_t {
	TYPE1, /* Type with MAC Address */
	TYPE2, /* Cryptographically random UUID */
	TYPE3, /* Globally sequential ID */
} ree_uuid_t;

typedef struct ree_uuid {
	ree_uuid_t type;
	char *field;
} ree_uuid;

ree_err_t urdb_generate_uuid(ree_uuid_t type);

#endif /* SRC_UUID_H_ */
