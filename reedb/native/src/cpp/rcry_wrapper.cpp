/*
 * cry_wrapper.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: spacekookie
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;

#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <cryptopp/twofish.h>
using CryptoPP::Twofish;

#include <cryptopp/ccm.h>
using CryptoPP::CTR_Mode;

#include <assert.h>

// Internal requirements
#include "../file_handle.h"
#include "rcry_wrapper.hpp"

/////////////////////////////
// This is where variables such as current crypto context IV and key will be stored.
// These HAVE to be freed and zero written when crypto context is removed
////////////////////////////
// \\// \\// \\// \\// \\// \\// \\// \\// \\// \\//

byte *vault_key;
struct ReeFile *context;

// \\// \\// \\// \\// \\// \\// \\// \\// \\// \\//

unsigned int generateKey(ree_crypto_t ctype, byte **key)
{
	AutoSeededRandomPool prng;

	if (ctype == RCRY_RIJNDAEL)
	{
		(*key) = key[AES::MAX_KEYLENGTH];
		prng.GenerateBlock((*key), sizeof((*key)));
	}
	else if (ctype == RCRY_TWOFISH)
	{
		(*key) = key[Twofish::MAX_KEYLENGTH];
		prng.GenerateBlock((*key), sizeof((*key)));

	}
	else
	{
		cerr
				<< "Unknown encryption scheme. Please pick one from the available or go fuck yourself!"
				<< endl;
		return 0xFA;
	}

	return 0;
}

unsigned int cryptoInit(ree_crypto_t ctype, byte **key)
{
	if (!vault_key) vault_key = (*key);
	else
	{
		cerr
				<< "There was already a key initialised! Terminate old context first!"
				<< endl;
		return 0xFB;
	}
	return 0;
}

unsigned int cryptoStop()
{
	free(vault_key);
	vault_key = NULL;

}
