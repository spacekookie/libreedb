#include <iostream>

#include <reedb/crypto/vault_token.h>
#include <reedb/vaults.h>

#include "crypto/rcry_context.h"
#include "ree_vault.h"

extern "C" {
#include "reedb/utils/helper.h"
}

/* Pointer storage for active and scoped vaults_interface */
static map<rdb_uuid *, ree_vault *> *active_vaults;
static vector<vault_meta> *scoped_vaults;

rdb_vaults::rdb_vaults() {
    this->id = "AAA";

    active_vaults = new map<rdb_uuid *, ree_vault *>();
    scoped_vaults = new vector<vault_meta>();
}

rdb_vaults::~rdb_vaults() {
    std::cout << "Closing interface: " << this->id << endl;

    // show content:
    for (std::map<rdb_uuid *, ree_vault *>::iterator it = active_vaults->begin(); it != active_vaults->end(); ++it) {
        it->second->close_vault();
        delete (it->second);
    }
}

vector<vault_meta> *rdb_vaults::list_vaults() {
    return scoped_vaults;
}

vault_meta *rdb_vaults::create(string name, string path, string passphrase) {

    cout << "Creating a new vault with name " << name << " and path " << path << endl;

    /* Create a token and UUID reference */
    rdb_token *token;
    vault_token_helper::create(&token, 0);

    cout << "Done with token!" << endl;

    rdb_uuid *uuid;
    uuid_helper::rdb_uuid_generate(&uuid, ONE);

    cout << "Done with UUID" << endl;

    rcry_engine *engine = new rcry_engine(55);
    rcry_context::instance()->add_engine(engine);

    /* Then pass everything we need down to the constructor */
    ree_vault *vault = new ree_vault(engine, name, path, passphrase);

    /* Create a vault meta to be scoped publicly */
    vault_meta meta;
    meta.id = uuid;
    meta.name = new string(name);
    meta.path = new string(path);
    meta.size = 0;
    meta.active = true;
    scoped_vaults->push_back(meta);

    /* Then mark our vault as active so that other people can use it */
    (*active_vaults)[uuid] = vault;

    vault_meta *to_return = new vault_meta(meta);
    return to_return;
}

rdb_token rdb_vaults::authenticate(rdb_uuid *id, string passphrase, bool permanent) {

    /** Attempt to unlock the vault */
    (*active_vaults)[id]->unlockVault(passphrase);
}
