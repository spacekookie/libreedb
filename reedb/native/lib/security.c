#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>
#include <string.h>
#include <stdbool.h>

// Internal (aka static) link to a base64 encoder.
#include "base64.h"
#include "error_codes.h"

#include "security.h"
/**
 * Global variable and constants definition here
 */
#define TARGET_CIPHER AES256_NACL
bool has_been_init = true;

/**
 * Simple function that takes a long string of binary OR base64 and decrypts it.
 * By default it handles binary input however if a boolean for base64 is provided (or value of 1)
 * it will also decode base64 on the fly and then decrypt.
 *
 * Takes a pointer to an encryption key and a pointer to a string_out array to write the
 * clear text into again. The cipher text is copied onto the stack.
 *
 * Can return non 0 values for error codes. (Look those up)
 */
unsigned int decryptAES(unsigned char **key, // Key for encryption. If no key is provided the decryption will fail.
		unsigned char **nonce, // Provide a pointer to the nonce for this data stream
		char *string_in, // Encrypted input string as Base64 or binary string (depending on bool base64).
		char **string_out, // Buffer for the decrypted string.
		bool base64) // Specify whether or not input buffer is base64 encoded.
{
	if (!(*key)) return ERROR_CRYPT_MISSING_KEY;

	// Defint the lengths for our data buffers.
	unsigned int cipher_t_length = strlen(string_in) * sizeof(unsigned char);
	unsigned int clear_t_length = crypto_secretbox_MACBYTES - cipher_t_length;

	// Set up the array for the cleartext here with the correct length
	unsigned char *cleartext = malloc(clear_t_length);

	/** MAGIC ZONE */
	crypto_secretbox_open_easy(cleartext, string_in, cipher_t_length, (*nonce),
			(*key));

	printf("Hello\n");
	/** MAGIC ZONE END */

	return 0;
}

/**
 * Encrypt a simple string with AES (NaCL implementation) and store it in an output array.
 * Either as binary or base64 encoded if the last parameter is true (or 1).
 *
 * Can return non 0 values for error codes (Look those up)
 */
unsigned int encryptAES(unsigned char **key, // Key for encryption. If none is provided one will be generated.
		unsigned char **nonce, // Provide a pointer to the nonce for this data stream. If none is provided it will generate one
		char *string_in, // Input array to encrypt.
		unsigned char **string_out, // Output buffer that the binary or base64 encrypted data will end in.
		bool base64) // Specify whether or not base64 encoding should be used automatically.
{
	if (!has_been_init) return ERROR_CRYPT_NO_INIT;

	// Set up how long the cipher text will be
	unsigned int input_size = strlen(string_in) * sizeof(unsigned char);
	unsigned int ciphertext_length = crypto_secretbox_MACBYTES + input_size;

	// Set up some cryptographic random nonsense
	if ((*nonce) == NULL)
	{
		(*nonce) = malloc(crypto_secretbox_NONCEBYTES);
		randombytes_buf(nonce, sizeof nonce);
	}

	// Generate a key if none was provided
	if ((*key) == NULL)
	{
		printf("[VERBOSE]: Generating key because none was provided\n");
		(*key) = calloc(crypto_secretbox_KEYBYTES, sizeof(unsigned char));
		randombytes_buf((*key), sizeof(*key));
	}

	// Set up the array for the ciphertext here with the correct length
	unsigned char ciphertext[ciphertext_length];

	/** == MAGIC ZONE == */

	// Puts the encrypted binary data in ciphertext buffer
	crypto_secretbox_easy(ciphertext, (unsigned char*) string_in, input_size,
			nonce, (*key));

	/** == END OF MAGIC ZONE == **/

	if (base64)
	{
		(*string_out) = (unsigned char*) base64_encode(ciphertext,
				sizeof(ciphertext));
	}
	else
	{
		(*string_out) = (unsigned char*) calloc(sizeof(ciphertext),
				sizeof(unsigned char));

		// Now the encrypted binary is in string_out
		memcpy((*string_out), ciphertext, sizeof(ciphertext));
	}

	// This crashes. So I assume something else has freed it already? But what could have?
	// TODO: Check if this has actually been freed before.
	// free(ciphertext);

	// Dump encrypted binary that's no longer needed from memory!
	memset(ciphertext, 0, sizeof(ciphertext) * sizeof(unsigned char));
	*ciphertext = NULL;

	// Return for success
	return 0;
}

int main(void)
{
	if (sodium_init() == -1)
	{
		fputs("Sodium Init Failed!\n", stderr);
		return 1;
	}
	int err;
	char *my_text =
			"This is my sentence I want to encrypt! Even longer strings! YAY!";
	unsigned char *encrypted;
	unsigned char *key = NULL;
	unsigned char *nonce = NULL;

	err = encryptAES(&key, &nonce, my_text, &encrypted, 1);

	// Check errors!
	if (err) fputs("An error has occured!", stderr);

	printf("Encrypted: %s\n", encrypted);

	// printf("My new shiny encryption key: %s", key);

	/** NOW GO AND DECRYPT THE DATA */
	unsigned char *decrypted;
	err = decryptAES(&key, &nonce, encrypted, &decrypted, 1);
	if (err) fputs("An error has occured!", stderr);

	return 0;
}

//#define MESSAGE ((const unsigned char *) "{'header':{'name':'Master', 'tags':['awesome', 'bdsm'],
//urls:['www.google.de']}, 'body':{'1::872372837':{'passphrase':'abcdefghijklmnop'}}}\0")
//#define MESSAGE_LEN (strlen(MESSAGE) * sizeof(char))
//#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)
//
//	unsigned char nonce[crypto_secretbox_NONCEBYTES];
//	unsigned char key[crypto_secretbox_KEYBYTES];
//	unsigned char ciphertext[CIPHERTEXT_LEN];
//
//	printf("Original:\t%s\n", MESSAGE);
//
//	randombytes_buf(nonce, sizeof nonce);
//	randombytes_buf(key, sizeof key);
//	crypto_secretbox_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce, key);
//
//	/** Change the binary to base64 */
//	char *base64_encrypted = base64_encode(ciphertext, sizeof(ciphertext));
//	// base64Encode(ciphertext, sizeof(ciphertext), &base64_encrypted);
//
//	printf("B64 Encrypted:\t%s\n", base64_encrypted);
//
//	size_t in_length;
//	unsigned char decrypted[MESSAGE_LEN];
//
//	char *binary_encrypted = base64_decode(base64_encrypted);
//	// base64Decode(base64_encrypted, &binary_encrypted, sizeof(base64_encrypted));
//
//	printf("Binary Encrypt:\t%s\n", binary_encrypted);
//
//	// Decrypt it
//	crypto_secretbox_open_easy(decrypted, binary_encrypted, CIPHERTEXT_LEN,
//			nonce, key);
//
//	// Log success!
//	printf("Decrypted:\t%s\n", decrypted);
//	printf("=== Done ===\n");
//
//	return 0;
//}
