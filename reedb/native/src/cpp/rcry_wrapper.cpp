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
#include "rcry_wrapper.hpp"

// C includes!
extern "C"
{
#include "../file_handle.h"
#include "../heads/base64.h"
}

/////////////////////////////
// This is where variables such as current crypto context IV and key will be stored.
// These HAVE to be freed and zero written when crypto context is removed
////////////////////////////
// \\// \\// \\// \\// \\// \\// \\// \\// \\// \\//

static byte *vault_key;
static struct ree_ccontext_t *context;
static ree_crypto_t target;

// \\// \\// \\// \\// \\// \\// \\// \\// \\// \\//

unsigned int rcry_generateKey(ree_crypto_t ctype, byte **key)
{
	AutoSeededRandomPool prng;

	if (ctype == RCRY_RIJNDAEL)
	{
		(*key) = (byte*) malloc(AES::MAX_KEYLENGTH);
		prng.GenerateBlock((*key), sizeof((*key)));
	}
	else if (ctype == RCRY_TWOFISH)
	{
		(*key) = (byte*) malloc(AES::MAX_KEYLENGTH);
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

unsigned int rcry_cryptoInit(ree_crypto_t ctype, byte **key)
{
	if (!vault_key) vault_key = (*key);
	else
	{
		cerr
				<< "There was already a key initialised! Terminate old context first!"
				<< endl;
		return 0xFB;
	}

	target = ctype;
	return 0;
}

unsigned int rcry_cryptoStop()
{

	// Kill the encryption key from memory
	// TODO: Test if it actually no longer shows up in coredump
	CryptoPP::SecureWipeArray(vault_key, sizeof(vault_key));
	vault_key = NULL;
}

unsigned int rcry_encryptInContext(byte *input, string *output)
{
	if (target == RCRY_RIJNDAEL)
	{
		try
		{
			CTR_Mode<AES>::Encryption e;
			e.SetKeyWithIV(vault_key, sizeof(vault_key), context->iv);
			StringSource(input, true,
					new StreamTransformationFilter(e, new StringSink(*output)) // StreamTransformationFilter
							);// StringSource
		} catch (const CryptoPP::Exception& e)
		{
			cerr << e.what() << endl;
			return 0x44;
		}
	}
	else
	{
		cerr << "Context not implemented! Use RCRY_RIJNAEL INSTEAD!" << endl;
		return 0xFAF;
	}

	return 0;
}

unsigned int rcry_decryptInContext(byte *input, string *output)
{
	if (target == RCRY_RIJNDAEL)
	{
		try
		{
			CTR_Mode<AES>::Decryption d;
			d.SetKeyWithIV(vault_key, sizeof(vault_key), context->iv);

			StringSource s(input, true,
					new StreamTransformationFilter(d, new StringSink(*output)) // StreamTransformationFilter
							);// StringSource

		} catch (const CryptoPP::Exception& e)
		{
			cerr << e.what() << endl;
			return 0x55;
		}
	}
	else
	{
		cerr << "Context not implemented! Use RCRY_RIJNAEL INSTEAD!" << endl;
		return 0xFAF;
	}

	return 0;
}

unsigned int rcry_setCryptoContext(struct ree_ccontext_t *newContext)
{
	if (newContext == NULL) return 0x65;

	// This means that a new IV needs to be made. Yay!
	if (newContext->fresh != 0)
	{
		byte *iv;

		if (target == RCRY_RIJNDAEL)
		{
			iv[AES::BLOCKSIZE];
		}
		else if (target == RCRY_TWOFISH)
		{
			iv[Twofish::MAX_KEYLENGTH];
		}
		else return 0x66;

		// Now there should be enough memory for a key!
		AutoSeededRandomPool prng;
		prng.GenerateBlock(newContext->iv, sizeof(newContext->iv));

		// Now malloc that bitch and assign the pointer to the struct
		byte *final_iv = (byte*) malloc(sizeof(iv));
		memcpy(final_iv, iv, sizeof(final_iv));
		newContext->iv = final_iv;

		// Make sure to update the fresh variable.
		newContext->fresh = 0;
	}
	string buffer;
	rcry_toBase64Converter(newContext->iv, &buffer, false);

	cout << "My IV: " << buffer << endl;

//	rcry_toBase64Converter(myFile->head, &buffer, true);

	// Now go ahead, zero the old context and write over the new!
	CryptoPP::SecureWipeArray(context, sizeof(context));
	context = newContext;
	return 0;
}

unsigned int rcry_toBase64Converter(byte *binary, string *base64, bool print)
{
	if (base64 == NULL) return 0xB0;

	(*base64).clear();
	StringSource(binary, sizeof((*binary)), true,
			new HexEncoder(new StringSink((*base64))));

	if (print) cout << "=> " << (*base64) << endl;
	return 0;
}

unsigned int rcry_FromBase64Converter(string *base64, byte **binary, bool print)
{
	if (binary == NULL) return 0xB1;
	(*binary) = (byte*) base64_decode((unsigned char*) (*base64).c_str());

	if (print) cout << "B=> " << (*binary) << endl;
	return 0;
}
