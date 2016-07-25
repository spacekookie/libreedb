#include "utils/uuid.h"

#include "crypto/rcry_utils.h"

#include <string.h>
#include <stdlib.h>

rdb_uuid *uuid_helper::generate() {

    /** Create a new UUID struct */
    rdb_uuid *uuid = new rdb_uuid { 0 };

    /** Generate random data in 4 packets */
    char *A, *B, *C, *D;
    rcry_utils::generate_weak_rand(&A, 8);
    rcry_utils::generate_weak_rand(&B, 8);
    rcry_utils::generate_weak_rand(&C, 6);
    rcry_utils::generate_weak_rand(&D, 8);

    /** Then copy our random data into our ID */
    strcpy((char*) uuid->x, A);
    strcat((char*) uuid->x, "-");
    strcat((char*) uuid->x, B);
    strcat((char*) uuid->x, "-");
    strcat((char*) uuid->x, C);
    strcat((char*) uuid->x, "-");
    strcat((char*) uuid->x, D);

      /** Then free our buffer arrays */
    free(A); A = NULL;
    free(B); B = NULL;
    free(C); C = NULL;
    free(D); D = NULL;

    return uuid;
}
