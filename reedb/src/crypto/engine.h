/* reedb - crypto/engine.h
 *
 * This header defines the API for the generic crypto engine interface.
 * It is built to handle multiple block ciphers without having to change
 * key storage or message authentication in the backend. Switching cipher
 * is as simple as calling a function!
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

/* Public definitions  */
#include "reedb/defs.h"

/* Internal data storage */
#include "datafile.h"

#ifndef SRC_CRYENGINE_H_
#define SRC_CRYENGINE_H_

typedef enum crytarget_t
{
	FILE_P,						// Indicates that a datafile struct is present
	STRING,						// Indicates that it's a simple string encryption.
	BSTRING, 					// Bytestrings because C!
} crytarget_t;

typedef enum cryflgs_t
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
} cryflgs_t;

/** The crypto context struct that is attached to a datafile */
typedef struct cry_context
{
	unsigned char			*block_key;
	unsigned char			*mac;
	unsigned long			nonce;
} cry_context;

ree_err_t init_rdb_crypto(enum cryflgs_t flags[]);

ree_err_t rdb_crycontext_switch(datafile *file, cryflgs_t flags[]);

ree_err_t rdb_encrypt(crytarget_t *type, void *data);

ree_err_t rdb_decryt(crytarget_t *type, void *data);

ree_err_t rdb_dump_key(char *scope);

#endif