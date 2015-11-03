/* reedb - crypto/engine.c
 *
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

/* Like *the* most important import ever :) */
#include <gcrypt.h>
#include <stdio.h>

/* Just some more stuff */
#include "reedb/defs.h"
#include "engine.h"

/* Some macro definitions for the crypto module */
#define RCRY_CORE_CIPHER GCRY_CIPHER_RIJNDAEL256 // This is a temporary workaround!
#define GCRYPT_NO_DEPRECATED
#define REQ_GCRYPT_VER "1.6.3"
#define SEC_MEM_SIZE 4096 // 4kb of memory. TODO: Be able to change this without recompiling

/* Static fields that hold crypto context so the lib doesn't have to */
static gcry_cipher_hd_t rcry_handle;

ree_err_t init_rdb_crypto(int flags)
{
	/* Version check should be the very first call because it
	 makes sure that important subsystems are initialized. */
	if (!gcry_check_version(GCRYPT_VERSION))
	{
		fputs("Reedb crypto engine error: libgcrypt version mismatch!\n", stderr);
		return CRYPTO_FAILED_BOOT;
	}

	/* We don't want to see any warnings, e.g. because we have not yet
		 parsed program options which might be used to suppress such
		 warnings. */
	gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);

	/* Allocate a pool of 32k secure memory.  This make the secure memory
		 available and also drops privileges where needed.  */
	gcry_control(GCRYCTL_INIT_SECMEM, SEC_MEM_SIZE, 0);

	/* It is now okay to let Libgcrypt complain when there was/is
		 a problem with the secure memory. */
	gcry_control(GCRYCTL_RESUME_SECMEM_WARN);

	/* Tell Libgcrypt that initialization has completed. */
	gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

	if (!gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P))
	{
		fputs("Reedb was unable to initialise the crypto engine...\n", stderr);
		return CRYPTO_FAILED_BOOT;
	}

	gcry_error_t open_error = gcry_cipher_open( 
		&rcry_handle, RCRY_CORE_CIPHER, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_SECURE | GCRY_CIPHER_CBC_MAC);

	if (open_error) {
		fputs("Reedb was unable to open the gcrypt cipher handle!\n", stderr);
		return CRYPTO_FAILED_BOOT;
	}

	return SUCCESS;
}

ree_err_t rcry_random_secure(unsigned char *(*value), size_t size, unsigned int rcry_rnd_level)
{
	enum gcry_random_level level;
	if(rcry_rnd_level == 0)			level = GCRY_WEAK_RANDOM;
	if(rcry_rnd_level == 1)			level = GCRY_STRONG_RANDOM;
	if(rcry_rnd_level == 999)		level = GCRY_VERY_STRONG_RANDOM;

	(*value) = gcry_random_bytes_secure(size, level);
	return SUCCESS;
}

/** Generate a key in secure memory with a bunch of flags 
 *  TODO: Actually do something with the flags! :) */ 
ree_err_t rcry_keygen(unsigned char *(*key), cryflgs_t flags)
{
	/* Get the required key length and malloc some secure memory for it */
	int ksize_t = gcry_cipher_get_algo_keylen(RCRY_CORE_CIPHER);
	(*key) = (unsigned char*) gcry_malloc_secure(ksize_t * sizeof(unsigned char));

	/* Then fill it with secure random garbage */
	gcry_randomize((*key), ksize_t, GCRY_STRONG_RANDOM);

	if(flags == AUTO_USE)
	{
		gcry_error_t key_error = gcry_cipher_setkey(rcry_handle, (*key), ksize_t);
		if (key_error) {
			fputs("An error occured when setting the key for context\n", stderr);
			return CRYPTO_UNKNOWN_FAILURE;
		}
	}
	return SUCCESS;
}

ree_err_t rcry_encrypt(crytarget_t type, void *data, unsigned char *(*output), unsigned char *key)
{
	if(type == FILE_P && key != NULL)
	{
		if(RDB_DEBUG) printf("[WARN]: When decrypting a file no key should be provided!\n");
	}

	if(type == STRING)
	{
		size_t text_length = strlen(data) + sizeof(unsigned char); // Null termination and all
		(*output) = gcry_malloc_secure(text_length);
		char *afterBuffer = gcry_malloc_secure(text_length);

		/* Set the key to the current one (just in case) */
		int ksize_t = gcry_cipher_get_algo_keylen(RCRY_CORE_CIPHER);
		gcry_error_t key_error = gcry_cipher_setkey(rcry_handle, key, ksize_t);
		if (key_error) {
			fputs("An error occured when setting the key for context\n", stderr);
			return CRYPTO_UNKNOWN_FAILURE;
		}

		gcry_error_t cry_err = gcry_cipher_encrypt(
			rcry_handle, (*output), text_length, data, text_length);

		if (cry_err) {
			printf("Encryption failure in module %s because of: %s\n", gcry_strsource(cry_err), gcry_strerror(cry_err));
			return CRYPTO_UNKNOWN_FAILURE;
		}

		// int index = 0;
		// printf("Our lovely key: ");
		// for (index = 0; index < text_length; index++)
		// 	printf("%02X", (unsigned char) tmp[index]);
		// printf("\n");

	} 
	else if(type == FILE_P)
	{
	  printf("Not yet implemented! Sorry :C\n");
	}

	return SUCCESS;
}

// void crypto() {

// 	int index = 0;
// 	for (index = 0; index < text_length; index++)
// 		printf("%02X", (unsigned char) encrypted[index]);
// 	printf("\n");

// 	// DECRYPT VALUE HERE

// 	gcry_error_t decrypt_error = gcry_cipher_decrypt(hdt, afterBuffer,
// 			text_length, encrypted, text_length);

// 	if (decrypt_error) {
// 		printf("gcry_cipher_decrypt failed:  %s/%s\n",
// 				gcry_strsource(decrypt_error), gcry_strerror(decrypt_error));
// 		return;
// 	}

// 	printf("%s", afterBuffer);
// 	printf("\n");

// 	// The close the cipher handle again
// 	gcry_cipher_close(hdt);
// }

// #define RCRY_CORE_CIPHER GCRY_CIPHER_RIJNDAEL256

// 	/* Get some secure random numbers as a key */
// 	unsigned char *key; // TODO: Replace this with something more...elegant?
// 	int success = rcry_random_secure(&key, gcry_cipher_get_algo_blklen(RCRY_CORE_CIPHER), 999);

// 	/* Initiate the cipher handle */
// 	gcry_cipher_hd_t *hd;
// 	int success = gcry_cipher_open(hd, RCRY_CORE_CIPHER, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_SECURE | GCRY_CIPHER_CBC_MAC);
// 	if(success != 0)
// 	{
// 		fputs("There was an error opening the cipher handle for encryption!\n", stderr);
// 		goto error_recovery;
// 	}

// 	success = gcry_cipher_setkey(hd, key, gcry_cipher_get_algo_blklen(RCRY_CORE_CIPHER));
// 	if(success != 0)
// 	{
// 		fputs("There was an error setting the key for encryption!\n", stderr);
// 		goto error_recovery;		
// 	}

// 	unsigned char *iv = malloc(sizeof(char) * 8);
// 	iv = "abcdefg\0";
// 	success = gcry_cipher_setiv(hd, iv, sizeof(char) * 8);
// 	{
// 		fputs("There was an error setting the CBC IV!\n", stderr);
// 		goto error_recovery;		
// 	}

// 	unsigned char *cipher_key;

// 	// Encrypts in buffer to out buffer (or just encrypts out in place if in is NULL and inlen is 0)
// 	success = gcry_cipher_encrypt(hd, cipher_key, size_t outsize, const unsigned char *in, size_tinlen)
