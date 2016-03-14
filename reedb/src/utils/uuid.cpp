#include "reedb/utils/uuid.h"
#include "crypto/rcry_utils.h"

#include <iostream>
#include <string.h>

using namespace std;

int uuid_helper::rdb_uuid_generate(rdb_uuid **uuid, uuid_type type) {
    (*uuid) = (struct rdb_uuid *) malloc(sizeof(struct rdb_uuid));
    (*uuid)->type = THREE;

    char *A = rcry_utils::generate_random(4, true);
    char *B = rcry_utils::generate_random(3, true);
    char *C = rcry_utils::generate_random(3, true);
    char *D = rcry_utils::generate_random(2, true);
    char *E = rcry_utils::generate_random(2, true);

    strcpy((*uuid)->id, A);
    strcat((*uuid)->id, "-");

    strcat((*uuid)->id, B);
    strcat((*uuid)->id, "-");

    strcat((*uuid)->id, C);
    strcat((*uuid)->id, "-");

    strcat((*uuid)->id, D);
    strcat((*uuid)->id, "-");

    strcat((*uuid)->id, E);

    cout << "UUID: " << (*uuid)->id << endl;
    return 0;
}
