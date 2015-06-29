#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>

// Preprocessor for the library
#define GCRYPT_NO_DEPRECATED
#define REQ_GCRYPT_VER "1.6.3"
#define SEC_MEM_SIZE 16384 // 16kb of secure memory. Too much?

void preInit() {

	// Check that the proper version is actually linked!
	if (!gcry_check_version(REQ_GCRYPT_VER)) {
		fputs("LibGCrypt version mismatch!\n", stderr);
		exit(2);
	}

	// Makes the warnings shut up
	gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);

	// TODO: Handle all other flags for the program here!

	// Init secure memory with predefined size
	gcry_control(GCRYCTL_INIT_SECMEM, SEC_MEM_SIZE, 0);

	// Now lets gcrypt talk to me again about warnings
	gcry_control(GCRYCTL_RESUME_SECMEM_WARN);

	// Tells LibGCrypt that init has been done
	gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
}

void crypto() {
#define MY_CIPHER GCRY_CIPHER_RIJNDAEL256

	// Set up an error handle
	gcry_cipher_hd_t hdt;
	gcry_error_t open_error = gcry_cipher_open(&hdt, MY_CIPHER,
			GCRY_CIPHER_MODE_ECB, GCRY_CIPHER_SECURE | 0);

	if (open_error) {
		fputs("Error opening the encryption handle!", stderr);
		exit(3);
	}

	int key_length = gcry_cipher_get_algo_keylen(MY_CIPHER);

	unsigned char *crypt_key = (unsigned char*) malloc(
			key_length * sizeof(char));

	//Use secure random number generation
	gcry_randomize(crypt_key, key_length, GCRY_STRONG_RANDOM);
	gcry_error_t key_error = gcry_cipher_setkey(hdt, &crypt_key, key_length);

	if (key_error) {
		fputs("Error allocating the key!", stderr);
		exit(4);
	}

	char *my_text = "This is a text that I want to encrypt";
	size_t text_length = strlen(my_text) + 1;
	char *encrypted = malloc(text_length);
	char *afterBuffer = malloc(text_length);

	printf("%s", my_text);
	printf("\n");

	gcry_error_t encryption_error = gcry_cipher_encrypt(hdt, encrypted,
			text_length, my_text, text_length);

	if (encryption_error) {
		printf("gcry_cipher_encrypt failed:  %s/%s\n",
				gcry_strsource(encryption_error),
				gcry_strerror(encryption_error));
		return;
	}

	int index = 0;
	for (index = 0; index < text_length; index++)
		printf("%02X", (unsigned char) encrypted[index]);
	printf("\n");

	// DECRYPT VALUE HERE

	gcry_error_t decrypt_error = gcry_cipher_decrypt(hdt, afterBuffer,
			text_length, encrypted, text_length);

	if (decrypt_error) {
		printf("gcry_cipher_decrypt failed:  %s/%s\n",
				gcry_strsource(decrypt_error), gcry_strerror(decrypt_error));
		return;
	}

	printf("%s", afterBuffer);
	printf("\n");

	// The close the cipher handle again
	gcry_cipher_close(hdt);

}

int main() {
	char *greeting = "Hello User!\0";
	char *farewell = "Goodbye User!\0";

	printf("%s\n", greeting);

// GCRYPT BETWEEN
	preInit();
	crypto();

// END

	printf("%s\n", farewell);
	return 0;
}
