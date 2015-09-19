
// Reedb includes
#include "reedb/utils/hashmap.h"

// System includes
#include <stdlib.h>
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
		int 			*iptr;
		char			*cptr;
		bool 			*bptr;
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

	//TODO: Stuff with our data. It's now in a struct. 
	// 			Decide what to do with it, encrypt it, then store


	// Free our data from the union, depending on what type it was.
	if(data->type == string) free(data->v.cptr);
	else if(data->type == boolean) free(data->v.bptr);
	else if(data->type == integer) free(data->v.iptr);

	// Finally free the data struct itself
	free(data);

	// And return the all clear.
	return 0;
}