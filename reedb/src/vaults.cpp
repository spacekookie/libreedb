#include "reedb/crypto/vault_token.h"
#include "crypto/rcry_context.h"
#include "reedb/vaults.h"
#include "ree_vault.h"

#include <algorithm>
#include <iostream>
#include <vector>

extern "C" {
#include "reedb/utils/helper.h"
}

using namespace std;

/* Vault UUIDs mapped to a listof tokens that are authenticated on it */
static map<rdb_uuid *, vector<rdb_token *> *> *token_map;

/* A list of UUIDs mapped to ree_vault objects */
static map<rdb_uuid *, ree_vault *> *active_vaults;

/* A list of vault_metas that are currently scoped */
static vector<vault_meta> *scoped_vaults;

rdb_vaults::rdb_vaults() {

    /* Give the interface a random ID */
    char *buf;
    rcry_utils::generate_weak_rand(&buf, 4);
    this->id = string(buf);

    token_map = new map<rdb_uuid *, vector<rdb_token *> *>();
    active_vaults = new map<rdb_uuid *, ree_vault *>();
    scoped_vaults = new vector<vault_meta>();
}

rdb_vaults::~rdb_vaults() {
    std::cout << "Closing interface: " << this->id << endl;

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

    rdb_uuid *uuid;
    uuid_helper::rdb_uuid_generate(&uuid, ONE);

    rcry_engine *engine = new rcry_engine(rcry_context::instance()->get_count_incr());
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

    /* Adding the vault meta to the scoped list */
    scoped_vaults->push_back(meta);

    /* Then mark our vault as active so that other people can use it */
    (*active_vaults)[uuid] = vault;

    /* Also make sure that the token list in the token_map exists */
    (*token_map)[uuid] = new vector<rdb_token *>();

    vault_meta *to_return = new vault_meta(meta);
    return to_return;
}

rdb_token rdb_vaults::authenticate(rdb_uuid *uuid, string passphrase, bool permanent) {

    /** Attempt to unlock the vault */
    try {
        int val = (*active_vaults)[uuid]->unlockVault(passphrase);

        if (val == 0) {
            cout << "Authentication successful!" << endl;

            /* Then generate an access token and return it to the user */
            rdb_token *token;
            vault_token_helper::create(&token, 0);

            /* Assign the token to the vault ID */
            (*token_map)[uuid]->push_back(token);

            /* Then return it to the user */
            rdb_token user;
            user = *token;
            return user;
        } else {
            cout << "Authentication failed because of error code " << val << endl;
            cout << "Check previous logging to get error details?" << endl;
        }

    } catch (runtime_error &e) {
        cout << "Error: " << e.what() << " occured!" << endl;
        cout << "Was your passphrase wrong?" << endl;
    }
}

void rdb_vaults::insert(rdb_uuid *id, rdb_token *token, string file_id, map<string, string> *content) {

    // TODO: Make this function a bit nicer
    bool accepted = false;

    /** Go and check every token for equal contents */
    for (rdb_token *t : *(*token_map)[id])
        if (strcmp(t->contents, token->contents) == 0) accepted = true;

    /* Make sure that we throw out any intruder */
    if (!accepted) {
        cout << "[ERROR] Token wasn't authorised for this vault!" << endl;
        return;
    }

    /* Create the file if it doesn't exists */
    if (!(*active_vaults)[id]->check_file_existance(file_id)) {

        (*active_vaults)[id]->add_file(file_id);
        cout << "Adding file '" << file_id << "'..." << endl;
    }

    /* Then either write into the new or existing file. But we don't care anymore at this point */
    (*active_vaults)[id]->update_file(file_id, content);

    cout << "File insertion complete" << endl;

}

void rdb_vaults::destroy(rdb_uuid *id, rdb_token *token) {
    // TODO: Make this function a bit nicer
    bool accepted = false;

    /** Go and check every token for equal contents */
    for (rdb_token *t : *(*token_map)[id])
        if (strcmp(t->contents, token->contents) == 0) accepted = true;

    /* Make sure that we throw out any intruder */
    if (!accepted) {
        cout << "[ERROR] Token wasn't authorised for this vault!" << endl;
        return;
    }

    cout << "Not implemented!" << endl;
}

void rdb_vaults::remove(rdb_uuid *id, rdb_token *token, string file_id, list <string> *fields) {
    // TODO: Make this function a bit nicer
    bool accepted = false;

    /** Go and check every token for equal contents */
    for (rdb_token *t : *(*token_map)[id])
        if (strcmp(t->contents, token->contents) == 0) accepted = true;

    /* Make sure that we throw out any intruder */
    if (!accepted) {
        cout << "[ERROR] Token wasn't authorised for this vault!" << endl;
        return;
    }

    /* Log and throw an error if the file doesn't exists */
    if (!(*active_vaults)[id]->check_file_existance(file_id)) {
        cout << "[ERROR] File '" << file_id << "' doesn't exists..." << endl;
        return;
    }

    // (*active_vaults)[id];
}