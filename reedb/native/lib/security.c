#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>
#include <string.h>
#include <stdbool.h>

// Internal (aka static) link to a base64 encoder.
#include "base64.h"
#include "error_codes.h"

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
 * Can return non 0 values for error codes. (Look those up)
 */
unsigned int decryptAES(unsigned char *key, // Key for encryption. If no key is provided the decryption will fail.
		char *string_in, // Encrypted input string as Base64 or binary string (depending on bool base64).
		char *string_out, // Decrypted string in buffer array.
		unsigned int s_size, // Manually specify input buffer size here.
		bool base64) // Specify whether or not input buffer is base64 encoded.
{
	if (!key) return ERROR_CRYPT_MISSING_KEY;

	return 0;
}

/**
 * Encrypt a simple string with AES (NaCL implementation) and store it in an output array.
 * Either as binary or base64 encoded if the last parameter is true (or 1).
 *
 * Can return non 0 values for error codes (Look those up)
 */
unsigned int encryptAES(unsigned char *key, // Key for encryption. If none is provided one will be generated.
		unsigned char *string_in, // Input array to encrypt.
		unsigned char *string_out, // Output buffer that the binary or base64 encrypted data will end in.
		unsigned int s_size, // Specify the length of the data manually here.
		bool base64) // Specify whether or not base64 encoding should be used automatically.
{
	if (!has_been_init) return ERROR_CRYPT_NO_INIT;

	printf("[VERBOSE]: Encryption has been init!\n");

	// Set up how long the cipher text will be
	unsigned int ciphertext_length = crypto_secretbox_MACBYTES + s_size;

	// Set up some criptographic random nonsense
	unsigned char nonce[crypto_secretbox_NONCEBYTES];
	randombytes_buf(nonce, sizeof nonce);

	// Generate a key if none was provided
	if (key == 0)
	{
		printf("[VERBOSE]: Generating key because none was provided\n");
		key = calloc(crypto_secretbox_KEYBYTES, sizeof(unsigned char));
		randombytes_buf(key, sizeof key);
	}

	// Set up the array for the ciphertext here with the correct length
	unsigned char ciphertext[ciphertext_length];

	/** == MAGIC ZONE == */

	// Puts the encrypted binary data in ciphertext buffer
	crypto_secretbox_easy(ciphertext, string_in, s_size, nonce, key);
	printf("[VERBOSE]: Encryption all done!\n");

	/** == END OF MAGIC ZONE == **/

	if (base64)
	{
		string_out = base64_encode(ciphertext, sizeof(ciphertext));
		printf("%s\n", string_out);
		// string_out = string_out[sizeof(tmp)];

		// memcpy(string_out, tmp, sizeof(string_out));
	}
	else
	{
		string_out = calloc(sizeof(ciphertext), sizeof(char));

		// Now the encrypted binary is in string_out
		memcpy(string_out, ciphertext, sizeof(ciphertext));
	}

	// Free some resources to avoid memory leak.
	free(ciphertext);
	free(nonce);

	// Return for success
	return 0;
}

int main(void)
{
	printf("[VERBOSE]: First!\n");
	if (sodium_init() == -1)
	{
		fputs("Sodium Init Failed!\n", stderr);
		return 1;
	}

	printf("[VERBOSE]: Init done!\n");

	char *my_text =
			"{'header':{'name':'Master', 'tags':['awesome', 'bdsm'], urls:['www.google.de']}, 'body':{'1::872372837':{'passphrase':'abcdefghijklmnop'}}}\0";
	char *encrypted;
	encryptAES(0, my_text, encrypted, sizeof(my_text), 1);

	printf("[VERBOSE]: Back home!\n");

	printf("Encrypted: %s\n", encrypted);

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
