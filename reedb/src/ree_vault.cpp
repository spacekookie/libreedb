// Reedb includes
#include "ree_vault.h"
#include "reedb/utils/errors.h"
#include "crypto/rcry_utils.h"

// Some runtime includes
#include <iostream>

using namespace std;

ree_vault::ree_vault(rdb_token *(*token), rdb_uuid *(*uuid), string name, string path, string passphrase)
{
  /* First generate and asign UUID */
  uuid_helper uh;
  *uuid = uh.generate();
  this->uuid = **uuid;
  
  /* Then generate and asign token */
  rdb_tokens_create(token, 0);
  
  // cout << "Our token is: " << (*token)->contents << endl;
  
  this->name = name;
  this->path = path;
  this->file_count = 0;
  
  cout << "Creating a vault, biatches!" << endl;
  
  this->headers = new map<string, datafile_h*>();
  this->files = new map<string, datafile*>();
  this->locks = new map<string, bool>();
  
  /* Now things required for quick header caching */
  this->h_fields = new map<string, void*>();
  
  /* Next up we need to generate a key */
  
}

ree_vault::ree_vault(rdb_uuid uuid, string path, string passphrase)
{

}
