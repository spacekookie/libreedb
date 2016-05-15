#ifndef REEDB_FILES_H
#define REEDB_FILES_H

typedef enum {
    CEY, // Used for key files. Sorry... 'cey' files
    REE, // Generic data files
    CFG, // Configurations
    CCH, // Cache files
} rdb_file_t;

/**
 * Takes a piece of data and two unique cryptographic values and dumps them as a base64
 * encoded string into a file at a very specifically defined location in a vault.
 *
 * @param salt: The salt used for this pw hash operation
 * @param iv: The IV for the crypto operation. These two are unique values
 * @param vault_path: Path of the vault on the file system
 * @param subcat: The category in the vault (e.g. "datastore")
 * @param name: The name of the file in clear text
 * @param type: The type of the file as one of the above options
 */
int rdb_files_crydump(char *salt, char *iv, char *data, char *vault_path, char *subcat, char *name, rdb_file_t type);

/**
 * @param name
 * @param path
 * @param data
 */
int rdb_files_dfdump(const char *name, const char *path, char *data);

#endif //REEDB_FILES_H
