/*
 * security.h
 *
 *  Created on: 30 Jun 2015
 *      Author: spacekookie
 */

#ifndef SECURITY_H_
#define SECURITY_H_

#define TRUE 1
#define FALSE 0

;
// Why the fuck...?
// bool has_been_init = false;

/**
 * Simple function that takes a long string of binary OR base64 and decrypts it.
 * By default it handles binary input however if a boolean for base64 is provided (or value of 1)
 * it will also decode base64 on the fly and then decrypt.
 *
 * Can return non 0 values for error codes. (Look those up)
 */
unsigned int decryptAES(unsigned char **key, // Key for encryption. If no key is provided the decryption will fail.
		unsigned char **nonce, // Provide a pointer to the nonce for this data stream. If none is provided it will generate one
		char *string_in, // Encrypted input string as Base64 or binary string (depending on bool base64).
		char **string_out, // Buffer for the decrypted string.
		bool base64); // Specify whether or not input buffer is base64 encoded.

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
		bool base64); // Specify whether or not base64 encoding should be used automatically.

#endif /* SECURITY_H_ */
