#include "ree_vault.h"
#include "reedb/utils/errors.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <cstring>

#include <wordexp.h>
#include <stddef.h>

// Cryptography includes
#include "crypto/rcry_engine.h"
#include "crypto/rcry_utils.h"
#include <cryptopp/aes.h>

extern "C" {
#include "reedb/utils/helper.h"
}

using namespace std;
using namespace boost;

ree_vault::ree_vault(rdb_token *(*token), rdb_uuid *(*uuid), rcry_engine *engine, string name, string path, string passphrase)
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
  engine->master_keygen(&master_key, nullptr);

  /** CREATE DIRECTORIES **/
  
  wordexp_t expantion;
  wordexp(path.c_str(), &expantion, 0);

  /* Expand our path, add the name as a folder and data dir */
  filesystem::path target(expantion.we_wordv[0]); 
  target /= name.c_str();
  target /= "datastore";
  filesystem::create_directories(target);
  
  target.remove_leaf() /= "keystore";
  filesystem::create_directories(target);

  target.remove_leaf() /= "checksum";
  filesystem::create_directories(target);

  target.remove_leaf() /= "metadata";
  filesystem::create_directories(target);
  
  wordfree(&expantion);

  /* Now write our default configuration to disk */
  
  /* Encrypt the key with the master passphrase and write that to disk */
  
}

ree_vault::ree_vault(rdb_uuid uuid, string path, string passphrase)
{

}
