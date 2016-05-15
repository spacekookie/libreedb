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
#include "rcry_token.h"

extern "C" {
#include "reedb/utils/helper.h"
#include "reedb/defs.h"
}

#include <cryptopp/aes.h>

using CryptoPP::AES;

#include <map>
#include <list>
#include <string>

/** Time required to start a job or next batch job */
#define RCRY_LOCK_TIMEOUT 250

typedef enum crytarget_t {
    FILE_P,           // Indicates that a datafile struct is present
    STRING,           // Indicates that it's a simple string encryption.
} crytarget_t;

typedef struct crycontext {
    byte zone[16];
    byte iv[CryptoPP::AES::BLOCKSIZE];
    size_t size;
    bool clear;
} crycontext;

typedef struct rcry_batch {
    unsigned int num_id;
    rcry_token *self;
} rcry_batch;

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
    std::map<rcry_token *, byte[AES::MAX_KEYLENGTH]> *context_map;

    /** Necessary for registered batch jobs */
    std::vector<rcry_batch> *batch_queue;

    /** Current context for everything to use*/
    crycontext *context;
    byte context_key[AES::MAX_KEYLENGTH];

    /** Locks the engine for exactly one job. Lock times out after N milliseconds */
    bool cry_lock = false;

public:

    /** Empty constructor that generates everything from scratch */
    rcry_engine(int id);

    rcry_engine(std::list<rdb_uuid> *ids);

    int query_id();

    /**
     * Prepare the crypto engine for a batch of jobs in continous and paralell use
     * from different encryption/ decryption targets.
     *
     * A token for the engine needs to be provided to start a batch. Said token will
     * then be registered in a batch-queue. Previous batches will be finished
     * before new ones are started.
     *
     * Calling this function can either be an INFO or BLOCKING call. INFO mode (block: false)
     * returns the number of job batches in the queue while BLOCKING will halt and wait
     * until the batch was successfully started.
     *
     *
     * @param token: The token for which a batch job should be created
     * @param block: Whether or not this function should be a blocking call
     * @returns unsigned int: Return value to indicate how many people are in queue before
     *                              or what errors occured while enqueueing.
     *
     */
    unsigned int start_batch(rcry_token *token, bool block);

    /**
     * Switch the engine into a different state for other tokens.
     *
     * !!! ALPHA SUPPORT FUNCTION !!!
     * This function allows for quick manual context switching inside the crypto engine.
     * It can do massive damage when called from a paralell thread or in the middle of another operation.
     *
     * This function will be removed! DO NOT IGNORE WARNINGS THROWN BY THIS FUNCTION!
     * Thanks
     */
    void switch_context(rcry_token *token);

    /**
     * Tells the crypto engine that the current batch for the provided token
     * has ended and it can be freed again for new operations.
     *
     * Finish your batch jobs! Letting them time out will reduce your tokens NICENESS.
     */
    unsigned int end_batch(rcry_token *token);

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

    /** Simple utility function to decrypt a C++ string */
    std::string encrypt_string(std::string data, crycontext *context);

    /** Simple utility function to decrypt a C++ string */
    std::string decrypt_string(std::string data, crycontext *context);

    /**
    * Encrypt in the current context with the current key scoped
    */
    char *encrypt(void *data, rcry_token *token);

    /**
    * Decrypt in the current context with the current key scoped
    */
    char *decrypt(void *data, rcry_token *token);

    /**
     * Alpha support function needed to get the key from the crypto engine
     * in encrypted form.
     * This is planned to be changed, however to get the system working, this
     * is the most elegant solution. DO _NOT_ ignore warnings thrown by this function!!
     * Thank you.
     *
     * @param salt: A pointer to an array that will hold a salt for the user passphrase
     * @param token: Your current token, just for security!
     * @param passphrase: The user passphrase used to encrypt the vault key.
     */
    std::string get_encrypted_key(char *(*salt), char *(*iv), rcry_token *token, std::string *passphrase);

    /**
     * Support function that checks a user passphrase by attempting to decrypt the master
     * key. Throws an exception if the decryption failed to indicate that the passphrase
     * was wrong.
     *
     * === Currently not implemented ===
     *
     * @param salt
     * @param iv
     * @param encrypted
     * @param passphrase
     */
    bool confirm_passphrase(char *(*salt), char *(*iv), char *encrypted, std::string *passphrase);

    /**
    * Initialise a vault on this crypto engine by giving it a master key
    * and binding it to a token. A vault can then switch crypto context
    * by providing it's token to the engine and then doing operations in
    * that crypto space.
    *
    * @param token: A token the key gets bound to
    */
    void init(rcry_token *token);

    /**
     * Alpha support function! Please don't ignore warnings thrown by this function.
     *
     * @param token: A pointer to be populated with a token
     * @param encrypted_key: The encrypted key pulled from a file
     * @param salt: The salt used to hash the user passphrase.
     * @param passphrase: The user passphrase to decrypt the key
     */
    void init(rcry_token *token, std::string encrypted_key, char *iv, char *salt, std::string passphrase);

    /** Hand in your token */
    void close();
};


#endif //REEDB_RCRYENGINE_H
