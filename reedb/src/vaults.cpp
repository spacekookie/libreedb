
extern "C" { 
#include "reedb/utils/uuid.h"
#include "reedb/utils/helper.h"
}

#include "reedb/vaults.h"

#include <iostream>

/* Pointer storage for active and scoped vaults */
// static map<rdb_uuid, ree_vault> *active_vaults;
// static vector<vault_head> *scoped_vaults;

rdb_vaults::rdb_vaults()
{
  std::cout << "Wooohoooo" << std::endl;
  id = "Silly id...";
}

rdb_vaults::~rdb_vaults()
{
  std::cout << "Closing interface: " << this->id << endl;
}

list<vault_head> *rdb_vaults::list_vaults()
{

}

vault_head* rdb_vaults::create(string name, string path, string passphrase)
{
  
}

