/*
 * uuid.h
 *
 *  Created on: 10 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_UUID_H_
#define SRC_UUID_H_

#include "utils.h"

typedef enum {
	TYPE1, /* Type with MAC Address */
	TYPE2, /* Cryptographically random UUID */
	TYPE3, /* Globally sequential ID */
} ree_uuid_t;

typedef struct ree_uuid {
	ree_uuid_t type;
	char *field;
} ree_uuid;

ree_err_t urdb_generate_uuid(ree_uuid_t type);

#endif /* SRC_UUID_H_ */
