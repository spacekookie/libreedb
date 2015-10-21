#include "ree_vault.h"

void create_folder_structure(char *name, char *path)
{
	int folder;
	char *new_name = rdb_concat_path_simple(name, ".reevault");
	char *master = rdb_concat_path_simple(path, new_name);
	free(new_name);

	/* Top directory */
	folder = mkdir(master, 0755);
	if(folder != 0)
	{
		if(RDB_DEBUG) printf("The path provided was not empty! Code %d\n", folder);
		return VAULT_CREATE_FAILED;
	}

	/* Create the keystore directory */
	char *keystore = rdb_concat_path_simple(master, "keystore");
	folder = mkdir(keystore, 0700);
	if(folder != 0) goto param_failure;
	free(keystore);

	/* Create the keystore directory */
	char *datastore = rdb_concat_path_simple(master, "datastore");
	folder = mkdir(datastore, 0755);
	if(folder != 0) goto param_failure;
	free(datastore);

	/* Create the keystore directory */
	char *parity = rdb_concat_path_simple(master, "parity");
	folder = mkdir(parity, 0755);
	if(folder != 0) goto param_failure;
	free(parity);

	/* Clean up after ourselves */
	free(master);
}

ree_err_t rdb_create_vault(vault *(*vault), ree_uuid *(*uuid), char *name, char *path, char *passphrase)
{
	/* Create the vault folders */
	create_folder_structure(name, path);

	/* Malloc a vault struct */
	(*vault) = malloc(sizeof(struct vault));
	if((*vault) == NULL) return MALLOC_FAILED;

	(*vault)->id = 

	return SUCCESS;
}

// typedef struct vault
// {
// 	/* Some metadata for the vault */
// 	char				id[32]; 					// UUID string
// 	size_t			size;							// number of files
// 	char				*name;						// User defined name
// 	char 				*path;						// Path on FS

// 	time_t			created;					// Date created
// 	time_t			modified;					// Date last modified

// 	/* Some crypto nonsense :) */
// 	map_t				*keystore;				// Map of files -> crypt-keys or regions -> crypt-keys
// 	map_t 			*tokens;					// Allowed access tokens

// 	/* Maps for data storage */
// 	map_t				*tags;						// Tags ordered by category (key)
// 	map_t				*files;						// Datafiles ordered by their name (key)
// } vault;