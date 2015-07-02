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
	NONE = 0xAAA, RCRY_RIJNDAEL = 0xAAB, RCRY_TWOFISH = 0xAAC
} ree_crypto_t;

unsigned int rcry_generateKey(ree_crypto_t ctype, byte **key);

unsigned int rcry_cryptoInit(ree_crypto_t ctype, byte **key);

unsigned int rcry_setCryptoContext(struct ree_ccontext_t *newContext);

unsigned int rcry_encryptInContext(string *input, string *output);

unsigned int rcry_decryptInContext(string *input, string *output);

unsigned int rcry_toBase64Converter(byte *binary, string *base64, bool print);

unsigned int rcry_FromBase64Converter(string *base64, byte *binary, bool print);

unsigned int rcry_cryptoStop();

#endif /* CRYPTO_WRAPPER_CRY_WRAPPER_HPP_ */
