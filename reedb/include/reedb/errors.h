/* libreedb - errors.h
 *
 * Errors are important for understanding why certain issues might
 * appear in your application. Reedb implements several error types
 * extending the std::runtime_error for you to catch.
 *
 * Each error contains a message describing the issue as well as
 * a well-defined error code.
 *
 * (c) 2015-2016                Lonely Robot.
 * Authors:                     Katharina 'spacekookie' Sabel
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
 * Additionally for error handling the function print_err_codes(...)
 * can be used to map all possible errors to their codes as they are
 * determined at compile-time of the application.
 *
 * Each error is thrown and must be handled by a catch-block. Internal
 * re-throws can sometimes mask errors from further inside the
 * database so please consult with your logs for more details.
 *
 * Good luck :)
 */

#ifndef REEDB_ERRORS_H
#define REEDB_ERRORS_H

/** Define some generic error codes first that we can propagate **/
typedef enum rdb_err_t {
    /* BOOLEAN STATES */
            TRUE, FALSE,                    // Have boolean values in a standard function return

    /* General purpose error codes */
            FAILURE = -1,                   // To be used when cause of error not known.
    SUCCESS = 0,                    // When something went according to plan.
    HUGE_SUCCESS = 0,               // Little Portal easter egg :)
    INVALID_PARAMS,                 // An invalid combination of parameters were provided
    MALLOC_FAILED,                  // Is returned when program failed to malloc.
    // Basically means there isn't enough RAM left.

    OS_NOT_SUPPORTED,               // Running on an OS that is currently not supported
    FILE_PARSE_FAILED,              // Indicates that parsing a config or datafile failed

    OPERATION_NOT_SUPPORTED,        // Something you're trying to do isn't supported. And might never be!
    // (See NOT IMPLEMENTED")
            NOT_IMPLEMENTED,                // Different to "NOT SUPPORTED" because it indicates missing
    // code base and might change.

    INVALID_PATH,                   // Path dprovided for config is invalid.
    SHORT_PASSPHRASE,               // Passphrase violates security standards
    NOT_INITIALISED,                // Tried to use Reedb without having initialised it first

    /* Vault error returns */
            VAULT_CREATE_FAILED,            // Something went wrong when creating a new vault.
    VAULT_ALREADY_SCOPED,           // Tried to scope a vault that was already in config
    VAULT_NOT_SCOPED,               // Tried to operate on vault that wasn't scope before
    VAULT_ALREADY_LOADED,           // Tried to load a vault that has already been loaded.
    VAULT_NOT_LOADED,               // Tried to operate on vault that isn't loaded yet.
    VAULT_DOESNT_EXIST,             // Tried to load vault that doesn't physically exist
    // anymore. This indicates a broken config!
            VAULT_ALREADY_EXISTS,           // Tried to create a vault that already exists on the FS.

    /* File specific failures */
            FILE_INSERT_FAILED,             // A generic failure return when a file can't accept new data
    FILE_LOCK_TIMEOUT,              // Inserting into a file timed-out because of a lock
    FILE_RM_FAILED,                 // Tried to remove a field from a version that doesn't exist.
    FILE_EMPTY_VERSION,             // Tried to finalise an empty version. Bad [boy|girl]!
    FILE_BAD_HEADER,                // Means that the header of a file is somehow damaged

    /* Crypto error returns */
            CRYPTO_FAILED_BOOT,             // An error occured while starting the crypto engine
    CRYPTO_INVALID_KEY,             // Returns when a crypto operation failed because the
    // wrong block cypher was applied.
            CRYPTO_MISSING_KEY,             // Key is NULL for some bizzare reason (This is not good)
    CRYPTO_CORRUPT_KEY,             // Key is corrupt for some more bizzare reason (This is even worse)
    CRYPTO_WRONG_BLOCKSIZE,         //
    CRYPTO_INVALID_CONTEXT,         // For some reason the crypto context is invalid.
    CRYPTO_WRONG_CONTEXT,           // Crypto context is invalid. Hints to someone trying to access files
    // they do not have access to via some weird hack!
            CRYPTO_MISSING_CRYRAM,          // We ran out of secure memory and can't allocate more in this session
    CRYPTO_UNKNOWN_FAILURE,         // A generic crypto failure.

    TEST_FAILED,                  // Indicates that a unittest has failed
    TEST_ALLOC_FAILED,            // Indicates that the allocation to a test suite failed
    TEST_NOT_VALID,               // Indicates that the testcase invoked invalid conditions

} rdb_err_t;

#ifdef __cplusplus
#include <stdexcept>

using std::runtime_error;

#include <string>

using std::string;

#include <reedb/utils.h>


class reedb_error : public runtime_error {
public:
    reedb_error(std::string msg, rdb_err_t c) : runtime_error(msg), code(c) {};

GET(rdb_err_t, code);
};

/** Generic error to throw if no other type fits */
class gen_rdberror : public reedb_error {
public:
    gen_rdberror(std::string msg, rdb_err_t c) : reedb_error(msg, c) {};

GET(rdb_err_t, code);
};

/** Error that can be throw when crypto operations fail */
class crypto_error : public reedb_error {
public:
    crypto_error(std::string msg, rdb_err_t c) : reedb_error(msg, c) {};

GET(rdb_err_t, code);
};

/** Error that can be thrown when dealing with writing data to disk or parsing files */
class storage_error : public reedb_error {
public:
    storage_error(std::string msg, rdb_err_t c) : reedb_error(msg, c) {};

GET(rdb_err_t, code);
};

/** An error that can be thrown when reading, parsing or updating the core Reedb config */
class config_error : public reedb_error {
public:
    config_error(std::string msg, rdb_err_t c) : reedb_error(msg, c) {};

GET(rdb_err_t, code);
};

#endif // C++ guard

#endif //REEDB_ERRORS_H
