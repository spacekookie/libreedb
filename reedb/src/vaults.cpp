#include "reedb/vaults.h"

#include "reedb/crypto/token.h"
#include "reedb/utils/uuid.h"
#include "crypto/rcry_engine.h"
#include "ree_vault.h"

#include <iostream>

extern "C" { 
#include "reedb/utils/helper.h"
}

/* Pointer storage for active and scoped vaults */
static map<rdb_uuid, ree_vault> *active_vaults;
static vector<vault_head> *scoped_vaults;

/* Pointer storage for main crypto engine */
static rcry_engine *engine;

rdb_vaults::rdb_vaults()
{
  this->id = "AAA";
  
  active_vaults = new map<rdb_uuid, ree_vault>();
  scoped_vaults = new vector<vault_head>();
  
  engine = new rcry_engine();
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

