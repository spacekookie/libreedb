/*
 * file_handle.h
 *
 *  Created on: 1 Jul 2015
 *      Author: spacekookie
 */

#ifndef SRC_FILE_HANDLE_H_
#define SRC_FILE_HANDLE_H_

#include <stdbool.h>

struct ree_ccontext_t
{
	bool fresh = 1;
	byte *iv;
	byte *mac;
};

/**
 * Basic file struct that holds an IV, head and body fields as well
 * as Message AuthentiCation tags for when they're being used.
 */
struct reefile_t
{
	/* Holds a crypto context for a file. mostly an IV and MAC */
	ree_ccontext_t *context;

	// This holds a tiger hash of the meta_name
	char *file_dump_name;

	byte *head;
	unsigned int headSize;

	byte *body;
	unsigned int bodySize;
};

struct raw_datafile_t
{
	char *meta_name;
	char *header;
	char *body;
};

/** Dumps data in a reefile_t struct to disk in the apropriate directory */
unsigned int dumpToDisk(unsigned char *name, struct reefile_t *data);

/** Writes data from a file into a reefile_t struct */
unsigned int pollFromDisk(unsigned char *name, struct reefile_t **data);

#endif /* SRC_FILE_HANDLE_H_ */
