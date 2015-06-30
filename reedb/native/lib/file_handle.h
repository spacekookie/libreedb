/*
 * file_handle.h
 *
 *  Created on: 30 Jun 2015
 *      Author: spacekookie
 */

#ifndef FILE_HANDLE_H_
#define FILE_HANDLE_H_

#include <stdbool.h>

/** Stores streams of data for header, body and a nonce for a file */
typedef struct data_dump
{
	/* Size markers for encryption */
	unsigned int paritySize;
	unsigned int dataSize;

	/* Data fields for the head, body, parity and crypto nonce */
	unsigned char *header;
	unsigned char *parity;
	unsigned char *nonce;
	unsigned char *body;
};

// unsigned int openFile(char *f, struct *data_dump, bool decrypt);

#endif /* FILE_HANDLE_H_ */
