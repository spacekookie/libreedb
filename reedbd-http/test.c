#include <stdio.h>
#include <stdbool.h>
#include <Judy.h>

#define MAXLINE 128

/* Define an ENUM that we will use to distinguish data types */
enum rdb_data_t { integer, string, boolean } rdb_data_t;

/* Define our struct that holds a type, size and generic memory field */
typedef struct rdb_gendata {

	enum rdb_data_t type;
	size_t					size;
	union value
	{
	   int					*iptr;
	   char					*sptr;
	   bool					*bptr;
	} value;
} rdb_gendata;

uint8_t   Index[MAXLINE];               // string to insert

/* Little helper function that creates our
 *  base Judy Array to work with.
 */
void createStructure(Pvoid_t **judy_array)
{
	/* After this point the judy collection can be used */
	(*judy_array) = (PWord_t)NULL;
	printf("Created a nice judy array!\n");

}

void add_entry(Pvoid_t *judy_array, char *name, char *data)
{
	printf("Inserting value '%s'=>'%s' into a judy array!\n", name, data);
	JSLI(data, judy_array, name);
}

int main()
{
		Pvoid_t		*judy_array;
		createStructure(&judy_array);

		// rdb_gendata *data = malloc(sizeof(struct rdb_gendata));
		// data->type = string;

		char *s_type = malloc(sizeof(char) * 9);
		s_type = "Triumph!";
		//data->value.sptr = s_type;

		/* Sanity check */
		printf("Sanity check: %s\n", s_type);

		add_entry(judy_array, "Reedb", s_type);

		// rdb_gendata *retrieved = malloc(sizeof(struct rdb_gendata));

		// char *d_type;
		// JSLF(s_type, judy_array, "\0");
		JSLG(s_type, judy_array, "Reedb");
		// JudySLGet(judy_array, d_type, "Reedb");

		printf("Get was successful...somewhat: %s\n", (s_type == NULL) ? "false" : "true");

		printf("Word of the day: %s\n", s_type);
		// printf("Other Word of the day: %s\n", &retrieved_tw->value.sptr);

    Pvoid_t   PJArray = (PWord_t)NULL;  // Judy array.
    PWord_t   PValue;                   // Judy array element.
    Word_t    Bytes;                    // size of JudySL array.

    int terminator = 0;

    while ( terminator <= 5 )
    {
    	terminator++;
    	fgets(Index, MAXLINE, stdin);
      JSLI(PValue, PJArray, Index);   // store string into array
      if (PValue == PJERR)            // if out of memory?
      {                               // so do something
	      printf("Malloc failed -- Buy more RAM!\n");
	      exit(1);
      }
      ++(*PValue);                    // count instances of string
    }
    Index[0] = '\0';                    // start with smallest string.
    JSLF(PValue, PJArray, Index);       // get first string

    printf("-------------\n"); 					// Print a little line for privacy
    
    while (PValue != NULL)
    {
        while ((*PValue)--)             // print duplicates
            printf("%s", Index);
        JSLN(PValue, PJArray, Index);   // get next string
    }
    JSLFA(Bytes, PJArray);              // free array

    fprintf(stderr, "The JudySL array used %lu bytes of memory\n", Bytes);
    return (0);
}


/*#include <stdio.h>
#include <string.h>
#include <stdbool.h> 


 
int main(void)
{
	struct rdb_gendata data;
	int a = 5;
	data.value.iptr = &a;

	printf("data: %i\n", *data.value.iptr);

   return 0;
}
*/
// /*
//  * test.c
//  *
//  *  Created on: 5 Aug 2015
//  *      Author: spacekookie
//  */

// #include <stdlib.h>
// #include <stdio.h>
// #include "reedb/core.h"
// #include "reedb/vault.h"

// #include "reedb/utils/hashmap.h"


// #define KEY_MAX_LENGTH (256)
// #define KEY_PREFIX ("somekey")
// #define KEY_COUNT (1024*1024)

// typedef struct data_struct_s
// {
//     char key_string[KEY_MAX_LENGTH];
//     char awesome[64];
//     int number;
// } data_struct_t;

// int main(void) {

//   int index = 0;
//   int error;
//   map_t mymap;
//   char key_string[KEY_MAX_LENGTH];
//   mymap = hashmap_new();
  
//   data_struct_t* value = malloc(sizeof(data_struct_t));
//   snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
  
// 	value->number = 1337;
// 	value->awesome = "This is awesome!";
// 	error = hashmap_put(mymap, value->key_string, value);

// 	ree_err_t rtn;
// 	reedb_c *container;
// 	rtn = rdb_set_passlength(16);
// 	rtn = rdb_set_verbose(true);
// 	rtn = rdb_set_path("/etc/reedb/");

// 	rtn = reedb_init(&container);
// 	if (rtn != HUGE_SUCCESS) {
// 		fputs("An error occurred when initialising Reedb!\n", stderr);
// 	} else {
// 		printf("Reedb was initialised successfully!\n");
// 	}
// 	reedb_init(&container);

// 	/** DO STUFF WITH LIB **/
// 	rdb_vault_init(&container);

// 	ree_token *token;
// 	ree_uuid *uuid;

// 	rdb_vault_create(&token, &uuid, "default", "~/Desktop",
// 			"megapassphrase123");

// 	/** NO LONGER DO STUFF WITH LIB **/
// 	reedb_terminate(&container, "Science");

// 	return 0;
// }
