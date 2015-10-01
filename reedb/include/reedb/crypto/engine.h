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
 * Author: Katharina 'spacekookie' Sabel
 *
 * Core header of the encryption engine. It can be given a type and
 * then handle whatever crypto implementation the user wants or is 
 * required by a vault to function properly. Please refer to the 
 * API docs and the Reedb manual to see what crypto implementation 
 * flags are available in what combination with other operand flags.
 *
 */

#ifndef SRC_CRYPTO_ENGINE_H_
#define SRC_CRYPTO_ENGINE_H_

/* Includes so we can work properly */
#include "reedb/utils.h"
#include "reedb/datafile.h"

typedef enum rdb_cryflgs_t
{
	RIJDAEL,
	TWOFISH,
	SERPENT,

	CRC,
	CTR,
	BLOCK_DUMP,
	STREAM_DUMP,

} rdb_cryflgs_t;

ree_err_t init_rdb_crypto(enum rdb_cryflgs_t flags[]);

ree_err_t rdb_crycontext_switch(rdb_datafile *file, rdb_cryflgs_t flags[]);

ree_err_t rdb_encrypt();

ree_err_t rdb_decryt();

ree_err_t rdb_dump_key(char *scope);


#endif
