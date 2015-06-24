/**
 * This file is part of Reedb. To learn more about it, go to:
 * www.github.com/tr-lonelyrobot/reedb
 *
 * It is being developed by (see contributers)
 * (c) 2015 Lonely Robot <www.lonelyrobot.io>
 *
 *=======================================================
 * Reedb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Reedb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Reedb.  If not, see <http://www.gnu.org/licenses/>.
 *=======================================================
 *
 * @author: Katharina 'spacekookie' Sabel <sabel.katharina@gmail.com>
 */

#include <stdio.h>
#include <gcrypt.h>
#include <assert.h>

/**
 * This function is used to test the gnu crypt functionality!
 * DO NOT USE THIS TEST IN PRODUCTION!
 */
void aesTest(int gcry_mode, char * iniVector) {
#define GCRY_CIPHER GCRY_CIPHER_RIJNDAEL256   // Pick the cipher here

	gcry_error_t gcryError;
	gcry_cipher_hd_t gcryCipherHd;
	size_t index;

	size_t keyLength = gcry_cipher_get_algo_keylen(GCRY_CIPHER);
	size_t blkLength = gcry_cipher_get_algo_blklen(GCRY_CIPHER);
	char * txtBuffer =
			"123456789 abcdefghijklmnopqrstuvwzyz ABCDEFGHIJKLMNOPQRSTUVWZYZ";
	size_t txtLength = strlen(txtBuffer) + 1; // string plus termination
	char * encBuffer = malloc(txtLength);
	char * outBuffer = malloc(txtLength);
	char * aesSymKey = "one test AES key"; // 16 bytes

	gcryError = gcry_cipher_open(&gcryCipherHd, // gcry_cipher_hd_t *
			GCRY_CIPHER,   // int
			gcry_mode,     // int
			0);            // unsigned int
	if (gcryError) {
		printf("gcry_cipher_open failed:  %s/%s\n", gcry_strsource(gcryError),
				gcry_strerror(gcryError));
		return;
	}

	gcryError = gcry_cipher_setkey(gcryCipherHd, aesSymKey, keyLength);
	if (gcryError) {
		printf("gcry_cipher_setkey failed:  %s/%s\n", gcry_strsource(gcryError),
				gcry_strerror(gcryError));
		return;
	}

	gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, blkLength);
	if (gcryError) {
		printf("gcry_cipher_setiv failed:  %s/%s\n", gcry_strsource(gcryError),
				gcry_strerror(gcryError));
		return;
	}

	gcryError = gcry_cipher_encrypt(gcryCipherHd, // gcry_cipher_hd_t
			encBuffer,    // void *
			txtLength,    // size_t
			txtBuffer,    // const void *
			txtLength);   // size_t
	if (gcryError) {
		printf("gcry_cipher_encrypt failed:  %s/%s\n",
				gcry_strsource(gcryError), gcry_strerror(gcryError));
		return;
	}

	gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, blkLength);
	if (gcryError) {
		printf("gcry_cipher_setiv failed:  %s/%s\n", gcry_strsource(gcryError),
				gcry_strerror(gcryError));
		return;
	}

	gcryError = gcry_cipher_decrypt(gcryCipherHd, // gcry_cipher_hd_t
			outBuffer,    // void *
			txtLength,    // size_t
			encBuffer,    // const void *
			txtLength);   // size_t
	if (gcryError) {
		printf("gcry_cipher_decrypt failed:  %s/%s\n",
				gcry_strsource(gcryError), gcry_strerror(gcryError));
		return;
	}

	printf("gcry_mode = %s\n",
			gcry_mode == GCRY_CIPHER_MODE_ECB ? "ECB" : "CBC");
	printf("keyLength = %d\n", keyLength);
	printf("blkLength = %d\n", blkLength);
	printf("txtLength = %d\n", txtLength);
	printf("aesSymKey = %s\n", aesSymKey);
	printf("iniVector = %s\n", iniVector);
	printf("txtBuffer = %s\n", txtBuffer);

	printf("encBuffer = ");
	for (index = 0; index < txtLength; index++)
		printf("%02X", (unsigned char) encBuffer[index]);
	printf("\n");

	printf("outBuffer = %s\n", outBuffer);

	// clean up after ourselves
	gcry_cipher_close(gcryCipherHd);
	free(encBuffer);
	free(outBuffer);
}

int main(void) {
	aesTest(GCRY_CIPHER_MODE_CBC, "a test ini value");
	//aesTest(GCRY_CIPHER_MODE_ECB, "different value!");
	//aesTest(GCRY_CIPHER_MODE_CBC, "a test ini value");
	//aesTest(GCRY_CIPHER_MODE_CBC, "different value!");
}
