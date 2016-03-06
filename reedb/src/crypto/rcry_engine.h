/* reedb - crypto/RCryEngine.h
 *
 * (c) 2015           Lonely Robot.
 * Authors:           Katharina 'spacekookie' Sabel
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
 * Main Reedb Crypto Engine class. It handles encryption, decryption, cipher selection,
 * key generation, expantion and message padding (where neccessary).
 */
#ifndef REEDB_RCRYENGINE_H
#define REEDB_RCRYENGINE_H

#include "reedb/utils/uuid.h"
#include "reedb/crypto/token.h"

extern "C" {
#include "reedb/utils/helper.h"
#include "reedb/defs.h"
}

#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <map>
#include <list>
#include <string>

typedef enum crytarget_t {
    FILE_P,           // Indicates that a datafile struct is present
    STRING,           // Indicates that it's a simple string encryption.
} crytarget_t;

typedef struct crycontext {
    byte key[32];
} crycontext;

typedef rdb_token rcry_token;

/**
 * Main crypto interface for Reedb. Is instantiated once and can handle
 * crypto for multiple vaults_interface. Vaults get assigned tokens and UUIDs needed
 * to switch the engine to their context. Then they can do crypto ops in that
 * context.
 * 
 * TODO: Make this multi-threadded as both libgcrypt and crypto++ are threadsafe.
 */
class rcry_engine {
  
private:
    std::map<rdb_token*, byte[AES::MAX_KEYLENGTH]> *context_map;
    crycontext *context;
    byte *context_key;
    bool cry_lock = false;

public:

    /** Empty constructor that generates everything from scratch */
    rcry_engine();
    rcry_engine(std::list<rdb_uuid> *ids);

    void switch_context(rdb_token *token);

    /**
    * Generate a master key for a vault with a specific token.
    * Token must be known and context loaded
    *
    * @param key: Reference pointer the key will be written into. Key is stored in secmem
    * @param token: Token given to the vault by this crypto engine
    *
    */
    void master_keygen(byte *key, rdb_uuid *uuid);

    /**_____________________
    * !! NOT IMPLEMENTED !! (See roadmap on github.com/reepass/reedb/wiki)
    *
    * Generate a key for a specific zone inside a vault. The zone needs to be specified
    * via its ID, the master key must be unlocked and the right context switch needs to be performed first.
    *
    * @param key: Reference pointer the key will be written into. Stored in secmem
    * @param zone: Zone identifier
    * @param token: The token for the vault
    *
    */
    void zone_keygen(unsigned char *(*key), std::string zone, rcry_token *token);

    /** Simple utility function to encrypt a C++ string */
    std::string *encrypt_string(std::string *data);

    /** Simple utility function to decrypt a C++ string */
    std::string *decrypt_string(std::string *data);

    /**
    * Encrypt in the current context with the current key scoped
    */
    unsigned char *encrypt(void *data);

    /**
    * Decrypt in the current context with the current key scoped
    */
    unsigned char *decrypt(void *data);

    /** Alpha support function needed to get the key from the crypto engine in encrypted form */
    void get_encrypted_key(unsigned char *(*key), std::string passphrase);

    /**
    * Initialise a vault on this crypto engine by giving it a master key
    * and binding it to a token. A vault can then switch crypto context
    * by providing it's token to the engine and then doing operations in
    * that crypto space.
    *
    * @param master_key: The master key of a vault
    * @param token: A token the key gets bound to
    */
    void init(rdb_token *token);

    /** Hand in your token */
    void close();
};


#endif //REEDB_RCRYENGINE_H
