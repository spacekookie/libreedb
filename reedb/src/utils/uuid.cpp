#include <reedb/reedb.h>
#include "crypto/rcry_utils.h"

#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

//int uuid_helper::rdb_uuid_generate(rdb_uuid **uuid, uuid_type type) {
//    (*uuid) = (struct rdb_uuid *) malloc(sizeof(struct rdb_uuid));
//    (*uuid)->type = THREE;
//
//    /** Generate random data in 4 packets */
//    char *A, *B, *C, *D;
//    rcry_utils::generate_weak_rand(&A, 4);
//    rcry_utils::generate_weak_rand(&B, 4);
//    rcry_utils::generate_weak_rand(&C, 3);
//    rcry_utils::generate_weak_rand(&D, 4);
//
//    /** Then copy our random data into our ID */
//    strcpy((*uuid)->id, A);
//    strcat((*uuid)->id, "-");
//    strcat((*uuid)->id, B);
//    strcat((*uuid)->id, "-");
//    strcat((*uuid)->id, C);
//    strcat((*uuid)->id, "-");
//    strcat((*uuid)->id, D);
//
//    /** Then free our buffer arrays */
//    free(A);
//    free(B);
//    free(C);
//    free(D);
//
//    /** Return 0 that everything went alright */
//    return 0;
//}
