/*
 * cry_wrapper.hpp
 *
 *  Created on: 30 Jun 2015
 *      Author: spacekookie
 */

#ifndef CRYPTO_WRAPPER_CRY_WRAPPER_HPP_
#define CRYPTO_WRAPPER_CRY_WRAPPER_HPP_

#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include "cryptopp/cryptlib.h"
#include "../file_handle.h"

/** Define an enum to remember the crypto modes that this module can do*/
typedef enum
{
	RCRY_RIJNDAEL, RCRY_TWOFISH
} ree_crypto_t;

unsigned int generateKey(ree_crypto_t ctype, byte **key);

unsigned int cryptoInit(ree_crypto_t ctype, byte **key);

unsigned int setCryptoContext(const struct ReeFile *newContext);

unsigned int encryptInContext(byte *input, unsigned char **output);

unsigned int decryptInContext(byte *input, unsigned char **output);

unsigned int cryptoStop();

#endif /* CRYPTO_WRAPPER_CRY_WRAPPER_HPP_ */
