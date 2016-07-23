//
// This file is part of libreedb.
//  It is free software under the LGPLv3

#ifndef REEDB_REEDB_H
#define REEDB_REEDB_H

#include <reedb/utils/helper.h>

#include <string>

using std::string;

#include <list>

using std::list;


/** Vault authentication token */
typedef struct rdb_token {
    char *contents;
    time_t created;
    unsigned int timed;
} rdb_token;

/**
 * @deprecated
 * Specifies the type of UUID used
 */
typedef enum uuid_type {
    ONE, TWO, THREE
} uuid_type;

/** UUID handle to identify vaults internally and to the user */
typedef struct rdb_uuid {
    char id[32];
    uuid_type type;
} rdb_uuid;

/* Contains metadata about a vault returned to the dev */
typedef struct {
    rdb_uuid *id;
    string *name, *path;
    unsigned int size;
    bool active;
} vault_meta;

/* Contains metadata fields about a file */
typedef struct {
    unsigned int revisions;
    string *category;
    string *title;
    list<string *> *tags;
} file_meta;

class rdb_core {
public:

/** Set a custom path for logs */
SETTER(string, log_path);

/** Set a custom path for configuration */
SETTER(string, cfg_path);

/** Enable verbose logging */
SETTER(bool, verbose);

/** Will ignore certain token errors */
SETTER(bool, permissive);

/** Defines how far this instance will scale */
ACCESS(size_t, max_scl);

    /** Gain information about all available vaults in this instance */
    list<vault_meta> list_vaults();

    /**
     * Add and remove vaults to and from the scope of this instance.
     *
     * The scope of what vaults developers can access depends on the
     * instance of the runtime. When using a managed runtime (for example)
     * the scope is usually the entire system and vaults are created in
     * a separate user space for authentication protection.
     *
     * When using libreedb outside of a managed context every single vault
     * the developer wants to access needs to be scoped into context first
     * before attempting to perform operations on it.
     *
     * @param head Meta struct describing location and type of vault
     */
    void scope_vault(vault_meta *head);

    /**
     * Removes a vault from this instances context again.
     *
     * @param id Vault management UUID for identification
     */
    void unscope_vault(rdb_uuid *id);

    /**
     * Gives the developer a working pointer on a fresh, unconfigured vault. From there
     * you can choose to initialise a default configuration or specify parameters
     * in the @link{#rdb_vault} interface to further customise the profile
     * and behaviour of your vault.
     *
     * By creating the vault via this interface it becomes scoped and manageble beyond
     * one application runtime.
     *
     * @param name A useful name to give the vault for identification
     * @param path The location on the file system to create the vault at
     * @return Pointer to a rdb_vault object to operate on
     */
    rdb_vault *start(string name, string path);


    /**
     * Destroy a vault, not just in scope but on the system. Requires previous
     * authentication and verification for rights. Can (will most likely) deny
     * your request.
     *
     * @param id Vault identification
     * @param token Authenticated token with sufficient rights
     */
    void destroy(rdb_uuid *id, rdb_token *token);

    /**
    * Authenticate a process for a vault and open it (if not already open).
    * Requires the vault ID and the master passphrase. Can generate timed
    * and permanent tokens. By default timed tokens are generated that
    * will loose their validity after the amount specified
    * in the master config of Reedb.
    *
    * @param id The id of the vault you want to authenticate for
    * @param passphrase The master passphrase of the vault
    * @param permanent Indicate whether the token should be permanent
    */
    rdb_token authenticate(rdb_uuid *id, string passphrase, bool permanent = false);

    /**
     * Function to yield in a token that you previously requested. It will throw a warning if
     * the token had already timed out. Will free the token pointer provided to the developer
     *
     * @param id: The id of the vault you want to yield access from
     * @param token: The token you want to yield. It will be freed.
     */
    rdb_token yield_token(rdb_uuid *id, rdb_token *token);

private:

};

/**
 * Accessor class that binds functions against core functionality in reedb to access
 * files, data and manage the instance of a database (called 'vault').
 */
class rdb_vault {

};

#endif //REEDB_REEDB_H
