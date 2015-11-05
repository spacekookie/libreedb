// This test file is part of Reedb.
// Do not use this code in production. It **may** be horribly broken.
// It comes no warranty of usefulness. Licenced under LGPL v3.0
//
// Author: Katharina 'spacekookie' Sabel


#include <gcrypt.h>
#include <stdio.h>

	/* Some macro definitions for the crypto module */
#define RCRY_CORE_CIPHER GCRY_CIPHER_RIJNDAEL256 // This is a temporary workaround!
#define GCRYPT_NO_DEPRECATED
#define REQ_GCRYPT_VER "1.6.3"
#define SEC_MEM_SIZE 4096 // 4kb of memory. TODO: Be able to change this without recompiling

int start_gcrypt()
{
	/* Version check should be the very first call because it
	 makes sure that important subsystems are initialized. */
	if (!gcry_check_version(GCRYPT_VERSION))
	{
		fputs("Reedb crypto engine error: libgcrypt version mismatch!\n", stderr);
		return 1;
	}

	gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);
	gcry_control(GCRYCTL_INIT_SECMEM, SEC_MEM_SIZE, 0);
	gcry_control(GCRYCTL_RESUME_SECMEM_WARN);

	/* Done with init */
	gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

	if (!gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P))
	{
		fputs("Failure when initialising gcrypt!\n", stderr);
		return 1;
	}

	return 0;
}

int encrypt(gcry_cipher_hd_t *hdt, char *clear, char *(*cipher_text), int text_length)
{
	(*cipher_text) = gcry_malloc_secure(text_length);
	gcry_error_t cry_err = gcry_cipher_encrypt((*hdt), (*cipher_text), text_length, clear, text_length);

	if (cry_err) {
		printf("Encryption with '%s' failed because of: %s\n", gcry_strsource(cry_err), gcry_strerror(cry_err));
		return 2;
	}
	return 0;
}

int decrypt(gcry_cipher_hd_t *hdt, char *cipher_text, char *(*clear))
{
	return 0;	
}

int test_crypto()
{
	gcry_cipher_hd_t hdt;
	gcry_error_t error = gcry_cipher_open(&hdt, RCRY_CORE_CIPHER, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_SECURE | GCRY_CIPHER_CBC_MAC);
	if (error) {
		printf("Encryption failure in module '%s' because of: '%s'\n", gcry_strsource(error), gcry_strerror(error));
		return 1;
	}


	/* Get the required key length and malloc some secure memory for it */
	int ksize_t = gcry_cipher_get_algo_keylen(RCRY_CORE_CIPHER);
	unsigned char *key = (unsigned char*) gcry_malloc_secure(ksize_t * sizeof(unsigned char));

	/* Then fill it with secure random garbage and then use it */
	gcry_randomize(key, ksize_t, GCRY_STRONG_RANDOM);
	error = gcry_cipher_setkey(hdt, key, ksize_t);
	if (error) {
		printf("Encryption failure in module '%s' because of: '%s'\n", gcry_strsource(error), gcry_strerror(error));
		return 1;
	}

	/*****************************************************************************
	 *	We have a key and a context handle now. We can start doing some crypto!  *
	 *****************************************************************************/

	unsigned char *cipher_text;
	unsigned char tmp[] = "Any old useless data will do...Any old useless data will do...Any old useless data will do...Any old useless data will do......\n";
	int text_length = strlen(tmp) * sizeof(unsigned char);
	unsigned char *data = malloc(text_length);
	strcpy(data, tmp);
	printf("Text length: %d\n", strlen(tmp) * sizeof(unsigned char));

	// unsigned char tmp[] = "{header:{name: 'Something Clever', tags: ['clever', 'awesome', 'yay!'], category: 'social', version:'2'}, body:{2:{url:'www.somethingawesome.org', username:'spacekookie', passphrase:'MargaretThatcherIs110percentSexy'}, 1:{passphrase:'passwerd'}}}";
	int encrypt_err;

	encrypt_err = encrypt(&hdt, data, &cipher_text, text_length);
	if(encrypt_err == 0)
	{
		int index;
		printf("Our Data was  : %s \n", data);
		printf("cipher text is: ");
		for (index = 0; index < text_length; index++) printf("%02X", (unsigned char) cipher_text[index]);
		printf("\n");
	}

	return 0;
}

static void aesTest(int gcry_mode, char * iniVector)
{
    #define GCRY_CIPHER GCRY_CIPHER_AES128   // Pick the cipher here
 
    gcry_error_t     gcryError;
    gcry_cipher_hd_t gcryCipherHd;
    size_t           index;
 
    size_t keyLength = gcry_cipher_get_algo_keylen(GCRY_CIPHER);
    size_t blkLength = gcry_cipher_get_algo_blklen(GCRY_CIPHER);
    char * txtBuffer = "123456789 abcdefghijklmnopqrstuvwzyz ABCDEFGHIJKLMNOPQRSTUVWZYZ";
    size_t txtLength = strlen(txtBuffer)+1; // string plus termination
    char * encBuffer = malloc(txtLength);
    char * outBuffer = malloc(txtLength);
    char * aesSymKey = "one test AES key"; // 16 bytes
 
    gcryError = gcry_cipher_open(
        &gcryCipherHd, // gcry_cipher_hd_t *
        GCRY_CIPHER,   // int
        gcry_mode,     // int
        0);            // unsigned int
    if (gcryError)
    {
        printf("gcry_cipher_open failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
 
    gcryError = gcry_cipher_setkey(gcryCipherHd, aesSymKey, keyLength);
    if (gcryError)
    {
        printf("gcry_cipher_setkey failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
 
    gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, blkLength);
    if (gcryError)
    {
        printf("gcry_cipher_setiv failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
 
    gcryError = gcry_cipher_encrypt(
        gcryCipherHd, // gcry_cipher_hd_t
        encBuffer,    // void *
        txtLength,    // size_t
        txtBuffer,    // const void *
        txtLength);   // size_t
    if (gcryError)
    {
        printf("gcry_cipher_encrypt failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
 
    gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, blkLength);
    if (gcryError)
    {
        printf("gcry_cipher_setiv failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
 
    gcryError = gcry_cipher_decrypt(
        gcryCipherHd, // gcry_cipher_hd_t
        outBuffer,    // void *
        txtLength,    // size_t
        encBuffer,    // const void *
        txtLength);   // size_t
    if (gcryError)
    {
        printf("gcry_cipher_decrypt failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
 
    printf("gcry_mode = %s\n", gcry_mode == GCRY_CIPHER_MODE_ECB ? "ECB" : "CBC");
    printf("keyLength = %d\n", keyLength);
    printf("blkLength = %d\n", blkLength);
    printf("txtLength = %d\n", txtLength);
    printf("aesSymKey = %s\n", aesSymKey);
    printf("iniVector = %s\n", iniVector);
    printf("txtBuffer = %s\n", txtBuffer);
 
    printf("encBuffer = ");
    for (index = 0; index<txtLength; index++)
        printf("%02X", (unsigned char)encBuffer[index]);
    printf("\n");
 
    printf("outBuffer = %s\n", outBuffer);
 
    // clean up after ourselves
    gcry_cipher_close(gcryCipherHd);
    free(encBuffer);
    free(outBuffer);
}
 

int main()
{
	// test_crypto();
	aesTest(GCRY_CIPHER_MODE_ECB, "a test ini value");
	aesTest(GCRY_CIPHER_MODE_ECB, "different value!");
	aesTest(GCRY_CIPHER_MODE_CBC, "a test ini value");
	aesTest(GCRY_CIPHER_MODE_CBC, "different value!");
	return 0;
}