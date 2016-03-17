#include "reedb/utils/uuid.h"
#include "crypto/rcry_utils.h"

#include <iostream>
#include <string.h>
#include <unistd.h>


using namespace std;

int uuid_helper::rdb_uuid_generate(rdb_uuid **uuid, uuid_type type) {
    (*uuid) = (struct rdb_uuid *) malloc(sizeof(struct rdb_uuid));
    (*uuid)->type = THREE;

    cout << "Generating shit" << endl;

    char *A;
    rcry_utils::generate_weak_rand(&A, 16);

    cout << "Done generating shit: " << A << endl;

//    cout << "Raw token: " << A << "-" << B << "-" << C << "-" << D << "-" << D << "-" << E << endl;
    return 0;
}
