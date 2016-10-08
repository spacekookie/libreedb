
#include <stdlib.h>
#include <lzma.h>

#include "queue.h"


rdb_qitem *add_to_butt(rdb_qitem *item, void *payload, int prio);

rdb_qitem *get_butt_item(rdb_qitem *item);

rdb_err_t rdb_queue_init(rdb_queue **qu)
{
    (*qu) = (rdb_queue*) calloc(sizeof(rdb_queue), 1);
    if(*qu == NULL) return MALLOC_FAILED;

    return SUCCESS;
}


rdb_err_t rdb_queue_add(rdb_queue *qu, void *payload, int prio)
{
    /** If we write the first item into the queue */
    if(qu->head == NULL) {
        rdb_qitem *item = (rdb_qitem*) calloc(sizeof(rdb_qitem), 1);
        qu->head = item;

    } else {
        rdb_qitem *tail = add_to_butt(qu->head, payload, prio);
        qu->butt = tail;
    }

    return SUCCESS;
}

/**
 * Gets the currently highest priority item from the queue and removes it.
 * @param qu
 * @param payload
 * @return
 */
rdb_err_t rdb_queue_pop(rdb_queue *qu, void **payload)
{
    /* Save the butt payload */
    (*payload) = qu->butt->payload;

    /* Free the butt item */
    qu->butt->prev->next = NULL;
    free(qu->butt);

    /* Then re-point the butt pointer */
    rdb_qitem *item = get_butt_item(qu->head);
    qu->butt = item;

    return SUCCESS;
}

/**
 * Checks if an item is available in the queue. Returns ERROR if queue is empty
 *
 * @param qu
 * @return
 */
rdb_err_t rdb_queue_peek(rdb_queue *qu)
{
    return SUCCESS;
}


/**
 * Frees all queue elements recursively (frees payloads!)
 *
 * @param qu
 * @return
 */
rdb_err_t rdb_queue_free(rdb_queue *qu)
{
    free(qu);
}


rdb_qitem *add_to_butt(rdb_qitem *item, void *payload, int prio)
{
    if(item == NULL) {

        rdb_qitem *item = (rdb_qitem*) calloc(sizeof(rdb_qitem), 1);
        item->payload = payload;
        item->prio = prio;

        return item;
    } else {
        return add_to_butt(item->next, payload, prio);
    }
}

rdb_qitem *get_butt_item(rdb_qitem *item)
{
    if(item->next == NULL) {
        return item;
    } else {
        return get_butt_item(item->next);
    }
}