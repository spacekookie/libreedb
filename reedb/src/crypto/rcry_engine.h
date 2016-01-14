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

extern "C" {
#include "reedb/utils/helper.h"
#include "reedb/utils/uuid.h"
}

#include <map>

typedef enum rcryflgs_t {
  RIJDAEL,        // The default for symmetric cryptography
  TWOFISH,        // Schneier says 'hi'
  SERPENT,        // Second place in AES contest
  RSA,            // Not currently implemented. Uses asymmetric RSA encryption
  ECC,            // Shiny elyptic curve cryptography (future default)

  CBC,            // Default mode for symmetric crypto
  CRT,            // Good for symmetric stream dumps
  BLOCK_DUMP,     // Indicates that crypto should dump one block at a time
  STREAM_DUMP,    // Makes crypto behave like a stream cipher

  MIGHTY,         // Specifies the strength of a generated key to strong
  QUICK,          // Specifies the strength of a generated key to weak (but quick)
  AUTO,           // Indicate that a generated key should automatically be scoped
} cryflgs_t;

typedef enum crytarget_t {
  FILE_P,           // Indicates that a datafile struct is present
  STRING,           // Indicates that it's a simple string encryption.
} crytarget_t;

/**
 * Main encryption class of Reedb. Handles encryption objects, states and ciphers. It gets called from the C-binding
 * wrapper to make it compatible with the rest of the library.
 */
class rcry_engine {

public:

    rcry_engine();
    void set_flags(cryflgs_t flags[]);
    
    byte **keygen(rcryflgs_t *flags);
};


#endif //REEDB_RCRYENGINE_H
