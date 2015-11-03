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

	MIGHTY,						// Specifies the strength of a generated key to strong
	QUICK,						// Specifies the strength of a generated key to weak (but quick)
	AUTO_USE,					// Indicate that a generated key should automatically be scoped
} cryflgs_t;

/** The crypto context struct that is attached to a datafile */
typedef struct cry_context
{
	unsigned char			*block_key;
	unsigned char			*mac;
	unsigned long			nonce;
} cry_context;

/* Init secure memory and start the crypto backend (libgcrypt) */
ree_err_t init_rdb_crypto(int flags);

ree_err_t term_rdb_crypto();

/* Switch crypto context with a bunch of flags and load keys for a zone */
ree_err_t rcry_crycontext_switch(datafile *file, cryflgs_t flags[]);

/* NEW: Contains 25% more crypto than the competition! */
ree_err_t rcry_encrypt(crytarget_t type, void *data, unsigned char *(*output), unsigned char *key);

/* Makes jibberish user friendly again since 2015 */
ree_err_t rcry_decryt(crytarget_t *type, void *data, unsigned char *(*output), unsigned char *key);

/* Generate a key from a padded passphrase or padded sub-key */
ree_err_t rcry_keygen(unsigned char *(*key), cryflgs_t flags);

/* Dump keys from a scope from secure memory */
ree_err_t rcry_dump_key(unsigned char *scope);

/* Generates secure numbers as integers */
ree_err_t rcry_random_secure(unsigned char *(*value), size_t size, unsigned int rcry_rnd_level);

/* Signals that I'm done with writing corny comments :) */
#endif