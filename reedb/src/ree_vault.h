#ifndef REEDB_VAULT_H
#define REEDB_VAULT_H

#include <string>

#include "reedb/utils/uuid.h"

/** Internal Reedb C requirements */
// extern "C" {
// #include "reedb/uuid.h"
// #include "reedb/crypto/token.h"
// #include "reedb/defs.h"
// };

using namespace std;

class ree_vault {
private:
  
  /* Some metadata fields about the vault */
  struct rdb_uuid uuid;
  string name, path;
  size_t fileCount;
  
  /* Crypto stuff */
  unsigned char *cryptoKey[];
  
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
     * @param name: RdbVault name on the filesystem
     * @param path: Location of the vault on the filesystem
     * @param passphrase: Master lock passphrase for the data
     *
     */
    ree_vault(string name, string path, string passphrase);

    /**
     * Only creates a new vault object from an existing vault that was
     * previously scoped on the system. A UUID is used as an index from
     * main config file. If a vault is not properly scoped yet it needs
     * to be added to the scope via the central #{vault.h} interface.
     *
     * @param uuid: Existing vault identifier (unique to a system)
     * @param passphrase: Master lock passphrase to unlock the vault.
     */
    ree_vault(rdb_uuid uuid, string passphrase);

    /* Makes sure that all files are closed and keys are dumped */
    ~ree_vault();
    
    /**
     * Unlocks a vault that is already scoped and even loaded by a different
     * target. This is used to have multiple applications access the same
     * vault.
     */
    void unlockVault(string passphrase, char *(*token));

    /**
     * Lock a vault with an access token even if there are other
     * applications currently using it.
     */
    void lockVault(char *token);
    
    /**** FILE OPERATION FUNCTIONS ****/
    
    void readFile(string name);
    
    void addFile(string name);
    
    void removeFile(string name);
    
    void updateFile(string name, string data);

    /**** SOME FIELD ACCESSORS ****/
    string getName();
    
    string getPath();
    
    rdb_uuid getUUID();
    
    size_t getSize();
    
};

#endif //REEDB_VAULT_H
