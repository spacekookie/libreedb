/*
 * file_handle.h
 *
 *  Created on: 1 Jul 2015
 *      Author: spacekookie
 */

#ifndef SRC_FILE_HANDLE_H_
#define SRC_FILE_HANDLE_H_

/**
 * Basic file struct that holds an IV, head and body fields as well
 * as Message AuthentiCation tags for when they're being used.
 */
struct ReeFile
{
	byte *iv;
	byte *mac;

	byte *head;
	unsigned int headSize;

	byte *body;
	unsigned int bodySize;
};

#endif /* SRC_FILE_HANDLE_H_ */
