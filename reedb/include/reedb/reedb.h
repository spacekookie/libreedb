/* libreedb - reedb.h
 *
 * This file defines all the public core functions for libreedb. From
 * it you will be able to create management instances as well as
 * standalone vaults with the respective runtime and configuration flags.
 * For more information, please consult the official documentation or
 * the repository wiki.
 *
 * (c) 2016 					Lonely Robot.
 * Authors:						Katharina 'spacekookie' Sabel
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * -------------------------------------------
 *
 * A (hopefully) logical API :)
 */

#ifndef REEDB_REEDB_H
#define REEDB_REEDB_H

// Reedb internal requirements
#include <reedb/utils.h>

// Runtime requirements
#include <string>
using std::string;

#include <list>
using std::list;

#include <map>
using std::map;

#include <fstream>
using std::fstream;
using std::ios;

/** Some flags that can be used to configure a vault after creation **/
#define RDB_FLG_ROOT                    (1 << 0)
#define RDB_FLG_MULTI_USER              (1 << 1)
#define RDB_FLG_MANAGED                 (1 << 2)
#define RDB_FLG_ANARCHY                 (1 << 3)

#define RDB_FLG_CACHE_SECURE            (1 << 4)
#define RDB_FLG_CACHE_QUICK             (1 << 5)
#define RDB_FLG_CACHE_BUFFERED          (1 << 6)

#define RDB_FLG_WRITE_BLOCK             (1 << 7)
#define RDB_FLG_WRITE_TREE              (1 << 8)

#define RDB_FLG_SCALE_DYNAMIC           (1 << 9)
#define RDB_FLG_SCALE_ENDLESS           (1 << 10)

#define RDB_FLG_LOG_VERBOSE             (1 << 11)
#define RDB_FLG_LOG_INFO                (1 << 12)
#define RDB_FLG_LOG_WARN                (1 << 13)
#define RDB_FLG_LOG_ERROR               (1 << 14)
#define RDB_FLG_LOG_SILENT              (1 << 15)

#define RDB_FLG_FTR_SINGLE_RECORD       (1 << 16)
#define RDB_FLG_FTR_MULTI_RECORD        (1 << 17)
#define RDB_FLG_FTR_RECORD_TREE         (1 << 18)
#define RDB_FLG_FTR_DISABLE_LUT         (1 << 19)
#define RDB_FLG_FTR_DISABLE_RQL         (1 << 20)
#define RDB_FLG_FTR_PERMISSIVE          (1 << 21)
#define RDB_FLG_FTR_DISABLE_HEADERS     (1 << 22)

// #define RDB_FLAG_RQLSYNTAX_SIMPLE       (1 << 23)

#define RDB_ZONE_ROOT                   0x00000000
#define RDB_USER_ROOT                   0x00000000

/**** Struct & Class declarations ****/

/** Vault authentication token */
typedef struct rdb_token {
    char *contents;
    time_t created;
    unsigned int timed;
} rdb_token;

/** UUID handle to identify vaults internally and to the user */
typedef struct rdb_uuid { unsigned char x[32]; } rdb_uuid;

/* Contains metadata about a vault returned to the dev */
typedef struct {
    rdb_uuid id;
    string name, path;
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

/* Forward declare so we don't need to move code around */
class rdb_vault;

/**
 * Core reedb class. Acts as a management and utility layer between the developer
 * and the vault objects. Can be used to create properly context scoped vault
 * instances and retrieve them in future runtimes.
 *
 * Can also lead to more predictable behaviour with handling os specific tasks
 * such as default file names, config locations and available interfaces to
 * bind on.
 */
class rdb_core {
public:

    rdb_core();

    ~rdb_core();

/** Set a custom path for logs */
SET(string, log_path);

/** Set a custom path for configuration */
SET(string, cfg_path);

/** Enable verbose logging */
SET(bool, verbose);

/** Defines how far this instance will scale */
DATA(size_t, max_scl);

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
    rdb_vault *create_vault(string name, string path);

    /**
     * Retrieves a vault management class either from active scope or creates
     * it from previously scoped vaults to do work on.
     *
     * Please note that authentication is still required past this point!
     *
     * @param id Vault identification
     * @return Pointer to a rdb_vault object to operate on
     */
    rdb_vault *access_vault(rdb_uuid *id);

    /**
     * Destroy a vault, not just in scope but on the system. Requires previous
     * authentication and verification for rights. Can (will most likely) deny
     * your request.
     *
     * @param id Vault identification
     * @param token Authenticated token with sufficient rights
     */
    void destroy_vault(rdb_uuid *id, rdb_token *token);

private:
    map<rdb_uuid*, rdb_vault*>  *active_vaults;

    class rdb_cfg_wrapper *rdb_scope;

    /** Fields for load balancing & scalability */
    list<class rcry_engine*>    *r_cryptons;

    /** Some utility fields */
    fstream *input, *output;
};

/**
 * Accessor class that binds functions against core functionality in reedb to access
 * files, data and manage the instance of a database (called 'vault').
 */
class rdb_vault {
public:

    /** Creates an empty vault to work on */
    rdb_vault(string name, string path);

    /** Cleans all the vaults resources from memory */
    ~rdb_vault();

    /**
     * Provide runtime flags. Multiple flags can be provided with the
     * bitwise OR operator. Pleanse consult the documentation
     * for possible flags and recommended runtime parameters.
     *
     * This function can be run multiple times for ease of use!
     *
     * @param flags Provide configuration parameters in form of a number
     */
    void set_flags(long flags);

    /**
     * A zone is a collection of files that a collection of users have
     * access to. Zones can be created during runtime. Each zone has a numeric
     * ID that makes it unique inside it's own vault.
     *
     * Users can then get bound to zone IDs. By default the ID 0 is taken for
     * the root zone.
     *
     * @param name
     * @return
     */
    long add_zone(const char *name);

    /**
     * Adds a new user to the vault. The name in the vault must be unique.
     *
     * @param name Unique username
     * @param zones A list of zones to add the user to
     * @return The vault user ID
     */
    long add_user(const char *name, long zones);

    /**
     * Sets the passphrase for a specific user ID. ID 0 is by default always
     * the ROOT user. This function can only be used to do initial passphrase
     * setup
     *
     * @param user Unique user ID
     * @param passphrase
     */
    void set_login(long user, const char *passphrase);

    /**
     * Take currently loaded settings, finalise them and finish setup. Only
     * past this point are changes to the vault final and persistent!
     *
     * This function will throw detailed errors about wrong setup parameters
     */
    void finish_setup();

    /**
    * Authenticate a process for a vault and open it (if not already open).
    * Requires the vault ID and the master passphrase. Can generate timed
    * and permanent tokens. By default timed tokens are generated that
    * will loose their validity after the amount specified
    * in the master config of Reedb.
    *
    * @param id: The id of the vault you want to authenticate for
    * @param passphrase: The master passphrase of the vault
    * @param permanent: Indicate whether the token should be permanent
    */
    rdb_token authenticate(rdb_uuid *id, string passphrase, bool permanent = false);

    /**
     * Function to yield in a token that you previously requested. It will throw a warning if
     * the token had already timed out previously. Will free the token pointer provided to the user
     *
     * @param id: The uuid of the vault you want to yield your token to
     * @param token: The token you want to yield. It will be freed.
     *                  Do not use it afterwards!
     */
    rdb_token yield_token(rdb_uuid *id, rdb_token *token);

    /**
    * Search a vault like you would any database. A header query will search
    * the header parts of each datafile. For a successful header query
    *
    *
    * @param id: The id of the vault to query from
    * @param token: An authentication token for this vault
    * @param search: A search query in RQL (Reedb Query Language). If ReedbSQL is installed SQL can be used.
    */
    list<file_meta> query_headers(rdb_uuid *id, rdb_token *token, string search);

    /**
    * Get the current state of a file by iterating through all its revisions. Blanked
    * fields will be overridden (and the latest version of a field displayed).
    *
    * @param id: The id of the vault to query from
    * @param token: An authentication token for this vault
    * @param query: The name or id of the file to query from
    */
    map<string, string> *query_file(rdb_uuid *id, rdb_token *token, string query);

    //TODO: Add `void query_file_rev(...); `

    /**
    * Inserts data into a datafile. Requires a vault id and token to execute. Also
    * requires a file id and content map to insert into.
    * See the documentation for a full guide on file insertions and file history.
    *
    * @param id: The id of the vault to query from
    * @param token: An authentication token for this vault
    * @param file_id: The id of the file to insert into
    * @param content: A map of <string::string> of content to insert
    */
    void insert(rdb_uuid *id, rdb_token *token, string file_id, map<string, string> *content);

    /**
    * Remove fields from a file. In files that support versioning this will only
    * blank the latest version of a field but not remove it from previous revisions.
    *
    * See the documentation for a full guide on file insertions and file history.
    *
    * @param id: The id of the vault to query from
    * @param token: An authentication token for this vault
    * @param file_id: The id of the file to insert into
    * @param fields: A list of fields to blank from a file
    */
    void remove(rdb_uuid *id, rdb_token *token, string file_id, list<string> *fields);

    /**
     * Used to migrate the header format of vaults that are currently scoped by this interface.
     * A meta_delta is basically the name of a meta-field that should be inserted mapped to
     * its type.
     * If a meta should be removed set the type to "-1".
     *
     * @param meta_delta: How the header set should be migrated.
     */
    void migrate_headers(map<string, string> *meta_delta);


private:
GET(string, id)

    /** Forward declare ree_vault class so we don't need to expose it */
    class ree_vault *self;

    /** List of authorised tokens */
    list<rdb_token *> auth_tokens;
};

#endif //REEDB_REEDB_H
