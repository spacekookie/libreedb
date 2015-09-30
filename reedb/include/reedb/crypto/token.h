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
 * token.h
 *
 *  Created on: 10 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_TOKEN_H_
#define SRC_TOKEN_H_

#include "reedb/uuid.h"
#include <stdlib.h>

typedef struct ree_token {
	unsigned char contents[128];
	time_t				created;
} ree_token;

ree_err_t urdb_generate_token(ree_uuid *id, char *mac);

#endif /* SRC_TOKEN_H_ */
