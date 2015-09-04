/*
 * ree_vault.h
 *
 *  Created on: 25 Aug 2015
 *      Author: spacekookie
 */

#include <stdbool.h>
#include "crypto/token.h"
#include "datafile.h"
#include "defs.h"

#ifndef SRC_REE_VAULT_H_
#define SRC_REE_VAULT_H_

typedef struct ree_vault
{
	/* Some metadata for the vault */
	char[32]		id,
	size_t			size,
	char				*name,
	char 				*path,

	time_t			created,
	time_t			modified,

	/* Some crypto nonsense */
	htable			*tokens,

	/* Now the actual information */
	hmap				*files,
	hmap 				*tags,

	// id 				: string uuid, fixed length
	// size			:	size_t, 		 fixed length
	// datafiles	:	[ name : string ] => [ file : *datafile ], both pointers?

	// tags			:	[ tag_name : string ] => 
	// 									( [ value : string ] : [ file_name : string ] )

	// tokens		:	hashtable [token : string]
} ree_vault;

ree_err_t rdb_create_vault(char *name, char *path);

#endif /* SRC_REE_VAULT_H_ */
