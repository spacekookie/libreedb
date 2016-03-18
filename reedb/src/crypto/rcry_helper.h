#ifndef RCRY_HELPER_H
#define RCRY_HELPER_H

/**
 * This file was added for alpha support. This might
 * change, disappear or spontaniously combust.
 *
 * Truth is...we don't know. There...I said it.
 */

#include "rcry_engine.h"

class rcry_context {

    rcry_context();

    /* Hold the static context variable */
    static rcry_context *self;

    /* Some data we need to hold in our context */
    rcry_engine **engines;
    size_t count;
    bool ready;

public:

    ~rcry_context();

    /* Return a static instance of this context  */
    static rcry_context *instance();

    /* Update the list of crypto engines available */
    void rcry_context_update(rcry_engine **engines);

    /* Get the amount of engines & increment it */
    int get_count_incr();

    int get_count();

    /* Make sure that the context is set to ready */
    bool is_ready();
};

#endif
