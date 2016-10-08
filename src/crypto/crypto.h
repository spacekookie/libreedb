/* libreedb - crypto.h
 *
 * Describes the reedb internal crypto API built around libgcrypt. Provides easy
 * to use, high-level functions to create keys, handle user crypto contexts and
 * do encryption workloads in queues.
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
 */

#ifndef REEDB_CRYPTO_H
#define REEDB_CRYPTO_H

#include <reedb/errors.h>
#include <glob.h>
#include <stdbool.h>
#include "queue.h"

/** Determine the job target type */
typedef enum target_t{

    /* RSA encryption modes */
    RSA_ENC, RSA_DEC,

    /* */
    AES,

    /* Axolotls asymetric */
    AXOLOTL_ENC, AXOLOTL_DEC
} target_t;

/**
 * Reedb crypto target that is submitted to
 * an engine job queue.
 *
 * token - A validation token to verify fence
 * mode - Determine the cryptography mode used
 * key - The key used in encryption (Either pub, pri or sym)
 *
 */
typedef struct rcry_target {
    // rdb_uuid        *token;
    target_t        mode;

    unsigned char   *key;
    size_t          key_len;

    unsigned char   *data;
    size_t          data_len;
} rcry_target;


/** Holds the result data from a computation */
typedef struct rcry_result {
    target_t        mode;
    unsigned char   *data;

} rcry_result;

/**
 *
 */
typedef struct rcry_engine {
    unsigned char   *token_seed;
    size_t          seed_len;

    bool            realtime, lock;
    unsigned long   id;

    rdb_queue       *job_queue;
    rcry_target     **completed;

} rcry_engine;


/********************************************
 *
 * Engine manipulation functions. Work on a crypto engine state
 *   that is held in a vault. An engine is unique in a vault
 *   but can be worked on by multiple worker threads.
 *
 *
 ********************************************/


/**
 *
 * @param e
 * @return
 */
rdb_err_t rcry_engine_init(rcry_engine *(*e), bool realtime, unsigned char *seed, size_t seed_len);


/**
 * THREAD LOCKS THE ENGINE UNTIL ACTION IS COMPLETE!
 *
 * Provide a seed for this engine. If no seed is provided before the first job is processed, a seed
 * will be generated.
 *
 * It is generally regarded to be better to seed a crypto engine on startup instead of during runtime because
 * it reduces the time of thread-lock on jobs (on multiple threads) and increases the quality of entropy
 * available at seed generation time.
 *
 * The seed function also allows you to re-seed an engine during runtime between job batches to increase the
 * validity of the random data used in encryption tasks.
 *
 * @param e
 * @param seed
 * @param seed_len
 * @return
 */
rdb_err_t rcry_engine_seed(rcry_engine *e, size_t seed_len);


/**
 * Creates a new item in the job queue of an engine. This means that at a future time, the job will be completed.
 * The completion of a job can either be awaited with `engine_wait`,or periodically checked with `engine_fence(...)`.
 *
 * A job submission guarantees that in the future the job will be completed. However submitting a job A before a
 * job B doesn't guarantee that it will be completed in this order as well. You can set the engine to be "real time"
 * in which this guarantee can be made!
 *
 * Read more about crypto engine scheduling in the wiki.
 *
 * @param e             The engine a job is submitted to
 * @param id            Pointer to an ID we can fence later
 * @param type          Provide the type of job we submit
 *
 * @param key           The key involved in the crypto transaction
 * @param key_len       The length of the key for security
 * @param data          The data a crypto transaction is performed on
 * @param data_len      The length of the data for security
 * @return
 */
rdb_err_t rcry_engine_addjob(rcry_engine *e, unsigned long *id, target_t type, const unsigned char *key,
                                    size_t key_len, const unsigned char *data, size_t data_len);


/**
 *
 * @param e
 * @param id
 * @param blocking
 * @return
 */
rdb_err_t rcry_engine_waitfor(rcry_engine *e, unsigned long id);


/**
 * Create a fence on a job in the queue that is called when the job is complete.
 *
 * This means that you can register an async callback call into the crypto engine that is
 * able to retrieve the completed computation from the job buffer.
 *
 * The provided fence callback needs to take one parameter which is
 *
 * @param e
 * @param id
 * @param callback
 * @return
 */
rdb_err_t rcry_engine_fence(rcry_engine *e, unsigned long id, void *(*fence_callback)(rcry_engine*));


/**
 * Returns the result of a computation job and removes it from the engine cache.
 *
 * Be careful when calling this function from an invalid context which will invalidate
 * the result from cache.
 *
 * @param e
 * @param id
 * @param data
 * @return
 */
rdb_err_t rcry_engine_result(rcry_engine *e, unsigned long id, rcry_result **data);


/**
 * Compute the next job in the queue, blocking on this thread. Each job is an atomic unit with duplicated
 * initialisation data, this function can be called from multiple threads while being completely thread-safe.
 *
 * The only check that is performed is if the engine is still in a valid state, which is the case if the
 * provided pointer is not NULL
 *
 * @param e             The engine that should have work invoked next
 * @return
 */
rdb_err_t rcry_engine_nextjob(rcry_engine *e);


/**
 * Clean up all memory from a crypto engine.
 *
 * @param e
 * @return
 */
rdb_err_t rcry_engine_free(rcry_engine *e);


/********************************************
 *
 * Engine manipulation functions. Work on a crypto engine state
 *   that is held in a vault. An engine is unique in a vault
 *   but can be worked on by multiple worker threads.
 *
 *
 ********************************************/

rdb_err_t rcry_keygen_init();

#endif //REEDB_CRYPTO_H
