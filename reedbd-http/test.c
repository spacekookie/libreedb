/*
 * test.c
 *
 *  Created on: 5 Aug 2015
 *      Author: spacekookie
 */

#include <stdlib.h>
#include <stdio.h>
#include "reedb/core.h"
#include "reedb/vault.h"

#include "reedb/utils/hashmap.h"


#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*1024)

typedef struct data_struct_s
{
    char key_string[KEY_MAX_LENGTH];
    char awesome[64];
    int number;
} data_struct_t;

int main(void) {

  int index = 0;
  int error;
  map_t mymap;
  char key_string[KEY_MAX_LENGTH];
  mymap = hashmap_new();
  
  data_struct_t* value = malloc(sizeof(data_struct_t));
  snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
  
	value->number = 1337;
	value->awesome = "This is awesome!";
	error = hashmap_put(mymap, value->key_string, value);

	ree_err_t rtn;
	reedb_c *container;
	rtn = rdb_set_passlength(16);
	rtn = rdb_set_verbose(true);
	rtn = rdb_set_path("/etc/reedb/");

	rtn = reedb_init(&container);
	if (rtn != HUGE_SUCCESS) {
		fputs("An error occurred when initialising Reedb!\n", stderr);
	} else {
		printf("Reedb was initialised successfully!\n");
	}
	reedb_init(&container);

	/** DO STUFF WITH LIB **/
	rdb_vault_init(&container);

	ree_token *token;
	ree_uuid *uuid;

	rdb_vault_create(&token, &uuid, "default", "~/Desktop",
			"megapassphrase123");

	/** NO LONGER DO STUFF WITH LIB **/
	reedb_terminate(&container, "Science");

	return 0;
}
