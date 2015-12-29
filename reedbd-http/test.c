
// Reedb includes
#include "reedb/utils/hashmap.h"

// System includes
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Data type enum */
typedef enum rdb_data_t
{
	string, integer, boolean
} rdb_data_t;

/* Data struct */
typedef struct rdb_gendata
{
  rdb_data_t 	type;
  size_t			size; // > 1 means it's an array.

  union v
  {
    int	*iptr;
    char *cptr;
    bool *bptr;
  } v;

} rdb_gendata;

/* Main entry point for our binary */
int main(void)
{
	rdb_gendata *data = malloc(sizeof(rdb_gendata));
	data->type = string;
	char tmp[] = "This is an awesome passphrase!";

	// Allocate memory for our string dynamically and store in union
	data->v.cptr = malloc((strlen(tmp) + 1) * sizeof(char));
	strcpy(data->v.cptr, tmp);

	// Overwrite our temporary stack array for security reasons.
	memset(&tmp, 0, sizeof(tmp));

	// Sanity check
	printf("Sanity Check: %s\n", data->v.cptr);

	/* 
	 *	Next up we test the functionality of our hashmap implementation. 
	 */
	int success;

	// First we create it
	map_t *hashmap = hashmap_new();

	// Try to insert something in the sucker
	success = hashmap_put(hashmap, "Reedb", data);
	printf("Return code was: %d\n", success);

	rdb_gendata *after;

	success = hashmap_get(hashmap, "Reedb", &after);
	printf("Return code was: %d\n", success);

	printf("=========================\nI can prove it: %s\n", after->v.cptr);

	printf("=========================\nOrkz Orkz Orkz Orkz Orkz Orkz Orkz Orkz\n");

	// Free our data from the union, depending on what type it was.
	if(data->type == string) 				free(data->v.cptr);
	else if(data->type == boolean) 	free(data->v.bptr);
	else if(data->type == integer) 	free(data->v.iptr);

	// Finally free the data struct itself
	free(data);

	// Then make sure it's actually gone... *paranoid*
	printf("But after the nukes fly: %s\n", after->v.cptr);

	// And return the all clear.
	return 0;
}