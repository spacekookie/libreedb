/* reedb - crypto/hashes.h
 *
 * Header file to use the hash functions implemented by libgcrypt and others
 * I don't know what else this is supposed to say. It's pretty damn fucking
 * self explanatory :) It makes dealing with shitty libraries less shitty.
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

#include "reedb/defs.h"

#ifndef SRC_CRYTIGER2_H_
#define SRC_CRYTIGER2_H_

#define RCRY_SALT_LENGTH 16 						// Salt length in bytes

/* Used for file hashing */
ree_err_t rcry_hash_tiger2(unsigned char *word, unsigned char *hash, unsigned char *salt);

/* Used for integrity checking */
ree_err_t rcry_hash_sha256(unsigned char *word, unsigned char *hash, unsigned char *salt);

/* Used for key padding */
ree_err_t rcry_hash_sphinx(unsigned char *word, unsigned char *hash, unsigned char *salt);

#endif