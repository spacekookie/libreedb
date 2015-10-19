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

/* Includes so we can work properly */
#include "reedb/utils.h"
#include "reedb/datafile.h"

typedef enum rdb_crytarget_t
{
	FILE,							// Indicates that a datafile struct is present
	STRING,						// Indicates that it's a simple string encryption.
	BSTRING, 					// Bytestrings because C!
}

typedef enum rdb_cryflgs_t
{
	/* Listing ciphers by name just because AES can change! */
	RIJDAEL,					// The default
	TWOFISH,					// Schneier says 'hi'
	SERPENT,					// Second place in AES contest
	RSA,							// Not currently implemented. Uses asymmetric RSA encryption

	CBC,							// Default mode of operation
	CRT,							// Good for stream dumps
	BLOCK_DUMP,				// Indicates that crypto should dump one block at a time
	STREAM_DUMP,			// Makes crypto behave like a stream cipher
} rdb_cryflgs_t;

ree_err_t init_rdb_crypto(enum rdb_cryflgs_t flags[]);

ree_err_t rdb_crycontext_switch(rdb_datafile *file, rdb_cryflgs_t flags[]);

ree_err_t rdb_encrypt(rdb_crytarget_t *type, void *data);

ree_err_t rdb_decryt(rdb_crytarget_t *type, void *data);

ree_err_t rdb_dump_key(char *scope);

#endif