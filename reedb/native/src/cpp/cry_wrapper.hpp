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

enum CryptoType
{
	RIJNDAEL, TWOFISH, SERPENT, WEIRD
};

unsigned char *generateCryptoInit(CryptoType type, unsigned char **key,
		unsigned char **iv);

unsigned int switchCryptoContext(unsigned char **iv, unsigned int dataLength);

unsigned char *encryptInContext(unsigned char *input, unsigned char **output);

unsigned char *decryptInContext(unsigned char *input, unsigned char **output);

// ...

//byte key[AES::DEFAULT_KEYLENGTH], iv[AES::BLOCKSIZE];
//// initialize key and iv here
//CFB_Mode<AES>::Encryption cfbEncryption(key, AES::DEFAULT_KEYLENGTH, iv);

#endif /* CRYPTO_WRAPPER_CRY_WRAPPER_HPP_ */
