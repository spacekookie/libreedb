#include <stdio.h>
#include <gcrypt.h>

int main() {
	char iniVector[16];
	char *encBuffer = NULL;
	FILE *fp, *fpout;
	char *key = "topolino e minni";
	gcry_cipher_hd_t hd;
	int bufSize = 16, bytes, algo = GCRY_CIPHER_AES128, keyLength = 16,
			blkLength = 16;

	memset(iniVector, 0, 16);

	encBuffer = malloc(bufSize);

	fp = fopen("prova", "r");
	fpout = fopen("out", "w");

	gcry_cipher_open(&hd, algo, GCRY_CIPHER_MODE_CBC, 0);
	gcry_cipher_setkey(hd, key, keyLength);
	gcry_cipher_setiv(hd, iniVector, blkLength);

	while (!feof(fp)) {
		bytes = fread(encBuffer, 1, bufSize, fp);
		if (!bytes) break;
		while (bytes < bufSize)
			encBuffer[bytes++] = 0x0;
		gcry_cipher_encrypt(hd, encBuffer, bufSize, NULL, 0);
		bytes = fwrite(encBuffer, 1, bufSize, fpout);
	}
	gcry_cipher_close(hd);
	fclose(fp);
	fclose(fpout);

	// Decrypt. Same algo as before

	gcry_cipher_open(&hd, algo, GCRY_CIPHER_MODE_CBC, 0);
	gcry_cipher_setkey(hd, key, keyLength);
	gcry_cipher_setiv(hd, iniVector, blkLength);

	fp = fopen("out", "r");
	fpout = fopen("origdec", "w");
	while (!feof(fp)) {
		bytes = fread(encBuffer, 1, bufSize, fp);
		if (!bytes) break;
		gcry_cipher_decrypt(hd, encBuffer, bufSize, NULL, 0);
		bytes = fwrite(encBuffer, 1, bufSize, fpout);
	}
	gcry_cipher_close(hd);

	free(encBuffer);
	encBuffer = NULL;
	return 0;
}
