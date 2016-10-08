/* libreedb - keystore.h
 *
 * A keystore that gets attached to a vault. Internally it maps a user id to
 * a key container which is able to hold several keys associated with that
 * user account.
 *
 * Each container holds a pri/ pub keypair used for the sharing functionality
 * as well as a primary and secondary symmetric key.
 * It also stores the key types used and makes sure to sync data with the
 * backend storage.
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
 * Developer notes:
 *
 * Public and private keypairs are only used for the sharing of information
 * between users.
 *
 * The primary symmetric key is used to encrypt zone keys for a specific user
 * while the secondary key is based on the user passphrase and used to harden
 * the primary key and the PK private key against theft.
 *
 * Internally this keystore implements a hashmap and makes access to some
 * information easier.
 *
 * In the current version (0.12+) this store also handles write requests to disk
 * so that all persistent keys are always saved to disk. This job can be moved to
 * a syndication thread.
 */


#include <datastore/hashmap.h>
#include <reedb/errors.h>
#include <hashtable.h>

#ifndef REEDB_KEYSTORE_H
#define REEDB_KEYSTORE_H

typedef struct rcry_key_c {
    void        *pri, *pub;
    char        *primary, *secondary;
} rcry_key_c;

enum rcry_store_t {

    /* Asymetric key cryptography */
    PRIVATE, PUBLIC,

    /* Symetric key cryptography keys */
    PRIMARY, SECONDARY,

    /* Only usable as delete marker - purges entry from keystore */
    __ALL__
} rcry_store_t;

typedef struct rcry_keystore {
    map_t           *keystore;
    char            *store_path;
} rcry_keystore;

rdb_err_t rcry_keystore_init(rcry_keystore *ks, const char *path);

rdb_err_t rcry_keystore_add(rcry_keystore *ks, char *id, char *key, enum rcry_store_t type);

rdb_err_t rcry_keystore_get(rcry_keystore *ks, char *id, enum rcry_store_t type, char **key);

rdb_err_t rcry_keystore_remove(rcry_keystore *ks, char *id, enum rcry_store_t type);

rdb_err_t rcry_keystore_sync(rcry_keystore *ks);

rdb_err_t rcry_keystore_free(rcry_keystore *ks);

#endif //REEDB_KEYSTORE_H
