#include <reedb/reedb.h>
#include <stdio.h>
#include <reedb/data.h>

int main(void)
{
    rdb_err_t err;
    rdb_data *root;
    err = rdb_data_malloc(&root);
    printf("Malloc returned: %s\n", rdb_error_getmsg(&err));

    rdb_data *key, *value;
    err = rdb_data_mallocpair(root, &key, &value);
    printf("Pair add returned: %s\n", rdb_error_getmsg(&err));

    const char *s_key = "username";
    const char *s_val = "spacekookie";
    err = rdb_data_addliteral(key, s_key, REAL_STRLEN(s_key));
    printf("Add literal returned: %s\n", rdb_error_getmsg(&err));

    err = rdb_data_addliteral(value, s_val, REAL_STRLEN(s_val));
    printf("Add literal returned: %s\n", rdb_error_getmsg(&err));

    return SUCCESS;
}