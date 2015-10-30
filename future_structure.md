# Future Structure

Turns out that trying to write a C application like it's Ruby is a *really* bad idea. So here is an attempt to outline how Reedb in C could be written.
This document will be edited and expanded as needed to provide a good record of what Reedb is supposed to do and how it is structured.

# Vaults

A vault is a collection of datafiles that are unlocked with a shared passphrase and contained in a single folder.

```
struct ree_vault
{
	id 				: string uuid, fixed length
	size			:	size_t, 		 fixed length
	datafiles	:	[ name : string ] => [ file : *datafile ], both pointers?

	tags			:	[ tag_name : string ] => 
										( [ value : string ] : [ file_name : string ] )

	tokens		:	hashtable [token : string]

}
```

`id` is a uuid to address a vault.
`size` is an integer to just provide metadata
`datafiles` is a hashmap of string => datafile pointers that allows for constant lookup of datafiles with a name.
`tags` is a hashmap of string => hashmap of string values (possible tags) and string filenames (where they can be found)
`tokens` is a hashtable that just holds tokens that are allowed to operate on this vault.

# Datafile

A datafile is a collection of entry edits. The array contains 

```C
struct datafile
{
	name 			: string
	head 			: [ field : string ] => [ value : rdb_gendata ]
	body 			:	Array [ HashMap [ field_name : string ] => [ rdb_gendata* ] ]
}
```

rdb_gendata is a struct that holds a type, a size and a generic memory field to contain a pointer of possibly different types.
The type needs to be updated every time the data is changed to prevent misreadin of information.


```C
enum rdb_data_t { integer, string, boolean } rdb_data_t;

struct rdb_gendata {

	enum rdb_data_t type;
	int size; // Contains if contains a list.
	union value
	{
	   int 		*iptr;
	   char  	*sptr;
	   bool 	*bptr;
	} value;
};
```