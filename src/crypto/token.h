/*
 * token.h
 *
 *  Created on: 10 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_TOKEN_H_
#define SRC_TOKEN_H_

#include "../uuid.h"
#include <stdlib.h>

typedef struct ree_token {
	unsigned char *contents;
	size_t length;
} ree_token;

ree_err_t urdb_generate_token(ree_uuid *id, char *mac);

#endif /* SRC_TOKEN_H_ */
