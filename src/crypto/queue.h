/* libreedb - queue.h
 *
 * A task queue used in the crypto-engine to order incoming jobs according
 * to their priority.
 * You are able to retrieve the highest priority element from the queue and add
 * items either at the end or with a specific priority into the queue.
 *
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

#include <reedb/errors.h>

#ifndef REEDB_TASK_QUEUE_H
#define REEDB_TASK_QUEUE_H


typedef struct rdb_qitem {
    struct rdb_qitem    *next, *prev;
    void                *payload;
    int                 prio;
} rdb_qitem;


typedef struct rdb_queue {
    rdb_qitem   *head, *butt;
    unsigned int    size;
} rdb_queue;


/**
 * Creates a new, empty queue for data insertions
 * @param qu
 * @return
 */
rdb_err_t rdb_queue_init(rdb_queue **qu);

/**
 * Adds a new item into the queue with a certain priority. If you want to add
 * a new item without a priority (thus on the end of the queue), use priority -1
 *
 * @param qu
 * @param payload
 * @param prio
 * @return
 */
rdb_err_t rdb_queue_add(rdb_queue *qu, void *payload, int prio);

/**
 * Gets the currently highest priority item from the queue and removes it.
 * @param qu
 * @param payload
 * @return
 */
rdb_err_t rdb_queue_pop(rdb_queue *qu, void **payload);

/**
 * Checks if an item is available in the queue. Returns ERROR if queue is empty
 *
 * @param qu
 * @return
 */
rdb_err_t rdb_queue_peek(rdb_queue *qu);

/**
 * Frees all queue elements recursively (frees payloads!)
 *
 * @param qu
 * @return
 */
rdb_err_t rdb_queue_free(rdb_queue *qu);

#endif //REEDB_TASK_QUEUE_H
