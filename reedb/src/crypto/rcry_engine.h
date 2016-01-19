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

#include <map>
#include <list>
#include <string>

typedef enum crytarget_t {
  FILE_P,           // Indicates that a datafile struct is present
  STRING,           // Indicates that it's a simple string encryption.
} crytarget_t;

typedef struct crycontext {
  unsigned char *key;
  unsigned char *entropy;
  // ...
} crycontext;

typedef char rcry_token;

/**
 * Main crypto interface for Reedb. Is instantiated once and can handle
 * crypto for multiple vaults. Vaults get assigned tokens and UUIDs needed
 * to switch the engine to their context. Then they can do crypto ops in that
 * context.
 * 
 * TODO: Make this multi-threadded as both libgcrypt and crypto++ are threadsafe.
 */
class rcry_engine {
  
private:
  std::map<rdb_uuid*, rcry_token> *tokens;
  crycontext *context;

public:

  /** Empty constructor that generates everything from scratch */
  rcry_engine();
  rcry_engine(std::list<rdb_uuid> *ids);
  
  void switch_context(rcry_token *token,rdb_uuid *uuid);
  
  /** 
   * Generate a master key for a vault with a specific token. 
   * Token must be known and context loaded
   * 
   * @param key: Reference pointer the key will be written into. Key is stored in secmem
   * @param token: Token given to the vault by this crypto engine
   *
   */
  void master_keygen(unsigned char *(*key), rdb_uuid *uuid);
  
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
  
  /**
   * Encrypt in the current context with the current key scoped
   */
  unsigned char *encrypt(void *data);
  
  /**
   * Decrypt in the current context with the current key scoped
   */
  unsigned char *decrypt(void *data);
  
  /** Init a scope and get a token */
  void init();
  
  /** Hand in your token */
  void close();
};


#endif //REEDB_RCRYENGINE_H
