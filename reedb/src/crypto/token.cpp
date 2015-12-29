/*
 * token.c
 *
 *  Created on: 10 Aug 2015
 *      Author: spacekookie
 */

#include "reedb/crypto/token.h"
#include "crypto/rcry_engine.h"

#include <malloc.h>

rdb_token* rdb_tokens_create(unsigned int timed)
{
  rdb_token *token = malloc(sizeof(rdb_token));
  token->timed = timed;
  token->created = 0;
  
  
}
