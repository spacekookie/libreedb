
extern "C" { 
#include "reedb/utils/uuid.h"
#include "reedb/utils/helper.h"
#include "reedb/crypto/token.h"
}

#include "reedb/vaults.h"
#include "ree_vault.h"

#include <iostream>

/* Pointer storage for active and scoped vaults */
// static map<rdb_uuid, ree_vault> *active_vaults;
// static vector<vault_head> *scoped_vaults;

rdb_vaults::rdb_vaults()
{
  this->id = "AAA";
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
  /* Create a token and UUID reference */
  rdb_token *token;
  rdb_uuid *uuid;
  
  /* Then pass everything we need down to the constructor */
  ree_vault *vault = new ree_vault(&token, &uuid, name, path, passphrase);
}

