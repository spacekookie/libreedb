#ifndef REE_VAULT_H
#define REE_VAULT_H

// Runtime includes
#include <string>
#include <list>
#include <map>

// Internal Reedb includes
#include "datafile.h"

#include "crypto/rcry_engine.h"
#include "reedb/crypto/token.h"
#include "reedb/utils/uuid.h"

extern "C" {
#include "reedb/utils/helper.h"
}

using namespace std;

class ree_vault {
private:
  
  /* Some metadata fields about the vault */
  GETTER(rdb_uuid, uuid);
  GETTER(string, name)
  GETTER(string, path)
  GETTER(size_t, file_count)
  
  /* File management fields */
  map<string, datafile_h*> *headers;
  map<string, datafile*> *files;
  map<string, bool> *locks;
  
  /* Header fields */
  map<string, void*> *h_fields;
  
  unsigned char *master_key;
  rdb_token token;
  
public:

    /**
     * Creates a new Vault on the filesystem. Will throw a RdbAdvErrors
     * exception when that is not possible. The cause of the error will
     * be specified in the exception.
     *
     * Three parameters are required to create a vault. Name, path and
     * a master passphrase. Everything else (uuid, tokens, etc.) will
     * be generated for you.
     *
     * @param token: Reference pointer to a token to be created
     * @param uuid: Reference pointer to a uuid to be created
     * @param name: RdbVault name on the filesystem
     * @param path: Location of the vault on the filesystem
     * @param passphrase: Master lock passphrase for the data
     *
     */
    ree_vault(rdb_token *(*token), rdb_uuid *(*uuid), rcry_engine *engine, string name, string path, string passphrase);

    /**
     * Only creates a new vault object from an existing vault that was
     * previously scoped on the system. A UUID is used as an index from
     * main config file. If a vault is not properly scoped yet it needs
     * to be added to the scope via the central #{vault.h} interface.
     *
     * @param uuid: Existing vault identifier (unique to a system)
     * @param passphrase: Master lock passphrase to unlock the vault.
     */
    ree_vault(rdb_uuid uuid, string path, string passphrase);

    /* Makes sure that all files are closed and keys are dumped */
    ~ree_vault();
    
    /**
     * Unlocks a vault that is already scoped and even loaded by a different
     * target. This is used to have multiple applications access the same
     * vault.
     */
    char *(*unlockVault)(string passphrase);

    /**
     * Lock a vault with an access token even if there are other
     * applications currently using it.
     */
    void lock_vault(char *token);
    
    /**** FILE OPERATION FUNCTIONS ****/
    
    void read_file(string name);
    
    void add_file(string name);
    
    void remove_file(string name);
    
    void update_file(string name, string data);
    
    /** 
     * Search through the vault with RQL. If search is NULL all 
     * headers will be returned
     * 
     * @param search: Search query. See documentation on details
     * 			how to use Reedb Query Language (RQL).
     * 
     * @returns list<datafile_h>: selection of headers
     */
    list<datafile_h> search_headers(string search);
  
    /** Adds the ability to add custom fields to the header */
    void add_hfields(string name, void* type);
    void remove_hfields(string name);
    
};

#endif //REE_VAULT_H
