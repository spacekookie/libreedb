#ifndef REEDB_RCRY_TOKEN_H
#define REEDB_RCRY_TOKEN_H

#include <stdlib.h>

typedef struct {
    char 		*contents;
} rcry_token;

class rcry_token_helper {

public:
    static void create(rcry_token *(*token), unsigned int timed);

    static void destroy(rcry_token *token);

//    rcry_token create(unsigned int timed);
//    void destroy(rcry_token *token);
};

#endif
