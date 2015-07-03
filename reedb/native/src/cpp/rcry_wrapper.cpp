/*
 * cry_wrapper.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: spacekookie
 */

#include <cryptopp/config.h>
#include <cryptopp/misc.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cstdbool>
#include <cstring>

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

static bool init = false;
static byte *vault_key;
static struct ree_ccontext_t *context;
static ree_crypto_t target = NONE;

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
	if (init)
	{
		cerr
				<< "Reedb Crypt has already been initialised. Stop the old instance first before starting a new one!"
				<< endl;
		return -1;
	}

	if (!vault_key)
	{
		// Don't malloc the key because it's already malloced somewhere else!
		vault_key = (*key);
	}

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
	return 0;
}

unsigned int rcry_encryptInContext(string *input, string *output)
{
	if (target == RCRY_RIJNDAEL)
	{

		try
		{
			CTR_Mode<AES>::Encryption e;
			e.SetKeyWithIV(vault_key, sizeof(vault_key), context->iv);
			StringSource((*input), true,
					new StreamTransformationFilter(e,
							new StringSink((*output))));
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

unsigned int rcry_decryptInContext(string *input, string *output)
{
	if (target == RCRY_RIJNDAEL)
	{
		try
		{
			CTR_Mode<AES>::Decryption d;
			d.SetKeyWithIV(vault_key, sizeof(vault_key), context->iv);
			StringSource s((*input), true,
					new StreamTransformationFilter(d,
							new StringSink((*output))));

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
		// TODO: Figure out how to make this code less ugly!
		AutoSeededRandomPool prng;

		if (target == RCRY_RIJNDAEL)
		{
			byte iv[AES::BLOCKSIZE];
			prng.GenerateBlock(iv, sizeof(iv));

			byte *final_iv = (byte*) malloc(sizeof(iv));
			memcpy(final_iv, iv, sizeof(final_iv) * 2);
			newContext->iv = final_iv;

			CryptoPP::SecureWipeArray(iv, sizeof(iv));

		}
		else if (target == RCRY_TWOFISH)
		{
			byte iv[Twofish::BLOCKSIZE];
			prng.GenerateBlock(iv, sizeof(iv));
			byte *final_iv = (byte*) malloc(sizeof(iv));
			memcpy(final_iv, iv, sizeof(final_iv));
			newContext->iv = final_iv;

			CryptoPP::SecureWipeArray(iv, sizeof(iv));
		}
		else return 0x66;

		// Make sure to update the fresh variable.
		newContext->fresh = 0;
	}

	// Now go ahead, zero the old context and write over the new!
	CryptoPP::SecureWipeArray(&context, sizeof(context));
	context = newContext;

	cout << "TARGET: " << target << endl;

	return 0;
}

unsigned int rcry_toBase64Converter(byte *binary, string *base64, bool print)
{
	if (base64 == NULL) return 0xB0;

	// TODO: Figure out WHY?
	(*base64).clear();
	StringSource(binary, sizeof(binary) * 2, true,
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
