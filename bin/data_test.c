//////////////////////////////////////////////////
// Simple testing client
// TODO: move this to testing directory
//
////////////////////////////////////////////////

// All we need :)
#include <reedb/reedb.h>
#include <reedb/data.h>
#include <stdio.h>
#include "../src/utils/encoding.h"

int main2(int argc, char **argv)
{
    rdb_err_t err;
    rdb_data *root;
    err = rdb_data_malloc(&root);
    printf("Malloc returned: %d\n", err);

    int ctr = 0;
    for(ctr; ctr < 10000; ctr++) {
        rdb_data *d;
        err = rdb_data_addrecursive(root, &d);
        printf("Malloc #%d returned: %d\n", ctr, err);

        for(int a = 0; a < 50; a++) {
            rdb_data *inner;

            err = rdb_data_addrecursive(d, &inner);
            printf("Malloc #%d returned: %d\n", ctr, err);

            rdb_data_addnumeral(inner, a);
            printf("Literal #%d returned: %d\n", ctr, err);
        }

        {
            /* Base64 encoding test */

            const char *data = "Cool data string!";
            int data_len = (int) REAL_STRLEN(data);

            int enc_len = rdb_coding_base64enclen(data_len);
            char encoded[enc_len];

            rdb_coding_base64enc(encoded, data, (int) data_len);

            int dec_len = rdb_coding_base64declen(encoded);
            char decoded[dec_len];

            rdb_coding_base64dec(decoded, encoded);

            rdb_data_addliteral(d, encoded, REAL_STRLEN(encoded));
            printf("Literal #%d returned: %d\n", ctr, err);
        }
    }

    { /* Print data we collected */
        int a;
        for(a = 0; a < root->size; a++) {
            rdb_data *d = root->payload.recursive[a];

            printf("============\n");
            printf("Index: %d\n", a);
            printf("Type: %s\n", rdb_data_dtype(d->type));
            printf("U/S: %d/%d\n", d->used, d->size);
            printf("Payload: ");
            if(d->type == LITERAL) printf("%s", d->payload.literal);
            if(d->type == NUMERAL) printf("%d", d->payload.numeral);
            printf("\n");
        }
    }

    rdb_data_free(root);
    return 0;
}