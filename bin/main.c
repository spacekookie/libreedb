#include <reedb/reedb.h>
#include <reedb/data.h>

#include <stdio.h>

int main(void)
{
    rdb_err_t err;
    rdb_context ctx;
    err = rdb_ctx_init(&ctx);

//    rdb_vault *vault;
//    err = rdb_ctx_vaultctr(&ctx, &vault, "default", "~/Documents");

    err = rdb_ctx_free(&ctx);

    return err;
}