#ifndef VAULTS_H
#define VAULTS_H

extern "C" { // C includes
#include "reedb/utils/helper.h"
}

// Utility Reedb includes
#include "reedb/utils/uuid.h"
#include "reedb/crypto/vault_token.h"

// Runtime includes
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

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

class rdb_vaults {

private:

    /*  */
GETTER(string, id)

public:

    rdb_vaults();

    ~rdb_vaults();

    /* Used to register rcry_engine on this interface */
    // void register_rcry(rcry_context *content);

    vector<vault_meta> *list_vaults();

    void scope_vault(vault_meta *head);

    void unscope_vault(rdb_uuid *id);

    /* Create a new vault with a name, a location path and a master passphrase */
    vault_meta *create(string name, string path, string passphrase);

    /* Destroy a vault with a certain uuid. Requires an authentication token */
    void destroy(rdb_uuid *id, rdb_token *token);

    /* Closes a vault with a certain uuid. Requires an authentication token and requires the token to be accepted by the vault */
    void close(rdb_uuid *id, rdb_token *token);

    /**
     * Used to migrate the header format of vaults that are currently scoped by this interface.
     * A meta_delta is basically the name of a meta-field that should be inserted mapped to
     * its type.
     * If a meta should be removed set the type to "-1".
     *
     * @param meta_delta: How the header set should be migrated.
     */
    void migrate_headers(map<string, string> *meta_delta);

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
    * Search a vault like you would any database. A header query will search
    * the header parts of each datafile. For a successful header query
    *
    *
    * @param id: The id of the vault to query from
    * @param token: An authentication token for this vault
    * @param search: A search query in RQL (Reedb Query Language). If ReedbSQL is installed SQL can be used.
    */
    list <file_meta> query_headers(rdb_uuid *id, rdb_token *token, string search);

    /**
    * Get the current state of a file by iterating through all its revisions. Blanked
    * fields will be overridden (and the latest version of a field displayed).
    *
    * @param id: The id of the vault to query from
    * @param token: An authentication token for this vault
    * @param query: The name or id of the file to query from
    */
    map<string, string> query_file(rdb_uuid *id, rdb_token *token, string query);

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
    void remove(rdb_uuid *id, rdb_token *token, string file_id, list <string> *fields);
};

#endif // VAULTS_H
