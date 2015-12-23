
#include "reedb/utils/uuid.h"
#include "reedb/vaults.h"
#include "ree_vault.h"

/* Pointer storage for active and scoped vaults */
static map<rdb_uuid, ree_vault> *active_vaults;
static vector<vault_head> *scoped_vaults;

rdb_vaults::rdb_vaults(reedb *core)
{
  
}
