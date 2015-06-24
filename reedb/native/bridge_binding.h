/*
 * bridge_binding.h
 *
 *  Created on: 24 Jun 2015
 *      Author: spacekookie
 */

# include <stdbool.h>

#ifndef BRIDGE_BINDING_H_
#define BRIDGE_BINDING_H_
#define CIPHER_TYPE FILL

#endif

/** Used to initialise the module */
bool initModule(enum type);

/** Used to encrypt a simple string of data */
char *encryptString(char *input);

/* Used to decrypt an encrypted string of data with the cipher specified */
char *decryptString(char *inout, enum type);

/* Generate a key for a specific cipher */
char *generateKey(enum type);

/* Remove all instances of sensitive information from memory */
bool dumpModule(void);
