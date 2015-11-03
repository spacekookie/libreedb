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

ree_err_t init_rdb_crypto(enum cryflgs_t flags[])
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
	gcry_control(GCRYCTL_INIT_SECMEM, 32768, 0);

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

	return SUCCESS;
}

ree_err_t term_rdb_crypto()
{
	gcry_control(GCRYCTL_DISABLE_SECMEM, 0);
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
