#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

enum rdb_data_t { integer, string, boolean } rdb_data_t;

struct rdb_gendata {

	enum rdb_data_t type;
	union value
	{
	   int 		*iptr;
	   char  	*sptr;
	   bool 	*bptr;
	} value;
};
 
int main(void)
{
	struct rdb_gendata data;
	int a = 5;
	data.value.iptr = &a;

	printf("data: %i\n", *data.value.iptr);

   return 0;
}