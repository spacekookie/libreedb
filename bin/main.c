#include <reedb/reedb.h>
#include <reedb/data.h>

#include <stdio.h>

#include <stb_leakcheck.h>


int main(void)
{
    rdb_context ctx;
    rdb_ctx_init(&ctx);

    rdb_vault *vault;
    rdb_ctx_vaultctr(&ctx, &vault, "default", "~/Documents");

	stb_leakcheck_dumpmem();

    return 0;
}
