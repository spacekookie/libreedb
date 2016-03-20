#include "ree_vault.h"

#include "reedb/utils/errors.h"
#include <iostream>
#include <cstring>

// Advanced path handling
#include <boost/filesystem.hpp>
#include <wordexp.h>
#include <stddef.h>

// Time, machines and username
#include <unistd.h>
#include <chrono>

// Config handling
#include <libconfig.h++>

// Cryptography includes
#include "crypto/rcry_context.h"
#include "crypto/rcry_engine.h"
#include "crypto/rcry_token.h"
#include "crypto/rcry_utils.h"

extern "C" {
#include "reedb/utils/helper.h"
}

using namespace std;
using namespace boost;
using namespace libconfig;

#include <gcrypt.h>

ree_vault::ree_vault(rcry_engine *engine, string name, string path, string passphrase) {

    /* Assign name, path and size metadata */
    this->name = name;
    this->file_count = 0;

    this->headers = new map<string, datafile_h *>();
    this->files = new map<string, datafile *>();
    this->locks = new map<string, bool>();

    /* Now things required for quick header caching */
    this->h_fields = new map<string, void *>();

    ////////////////////////////////////////////////////////////////////////////////////
    ///////////////////// Create directory tree for keystore, datastore, etc.
    ////////////////////////////////////////////////////////////////////////////////////

    wordexp_t expantion;
    wordexp(path.c_str(), &expantion, 0);

    bool created = true;
    string combined = name;
    combined.append(".vault");

    /* Expand our path, add the name as a folder and data dir */
    filesystem::path target(expantion.we_wordv[0]);
    target /= combined.c_str();
    string full_path = target.c_str();

    // Save the full path
    this->path = full_path;

    target /= "datastore";
    created = filesystem::create_directories(target);

    if (created) {
        target.remove_leaf() /= "keystore";
        filesystem::create_directories(target);

        target.remove_leaf() /= "checksum";
        filesystem::create_directories(target);

        target.remove_leaf() /= "metadata";
        filesystem::create_directories(target);
        target.remove_leaf();
    } else {
        cout << "[ERROR] A vault already exists at that location!" << endl;
        wordfree(&expantion);
        throw 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///////////////////// Prepare the crypto initialisation & key generation
    ////////////////////////////////////////////////////////////////////////////////////

    /* Initialise the crypto engine for this vault */
    rcry_token *token;
    rcry_token_helper::create(&token, 0);

    /* Copy the pointer into our vault so we can access crypto after we leave this function :) */
    this->token = token;

    engine->init(token);
    engine->switch_context(token);

    cout << "Encrypting generated key with user password and salt" << endl;

    /* Then retrieve the key in encrypted form to write it to disk */
    string encrypted_key = engine->get_encrypted_key(&this->salt, &this->iv, token, &passphrase);

    /* Release the crypto engine again */
    engine->switch_context(nullptr);

    /* Hash the passphrase and store it in the vault for quick passphrase comparison */
    this->pw_hash = rcry_utils::md_sha256_salted(salt, passphrase.c_str(), true);

    cout << "Preparing to dump encrypted key to disk..." << endl;

    /* Generate our keyfile name here so we can use it in the metadata header */
    char *sub_path = "/keystore/";
    char *master = rcry_utils::md_sha256("master", true);

    size_t meta_size = 256;
    char concat[meta_size + strlen(encrypted_key.c_str())];

    // <SALT>::<IV>::<USER>::<ZONE>::<DATA>
    strcpy(concat, this->salt);         // Salt (Optional)
    strcat(concat, "::");
    strcat(concat, this->iv);           // Encryption IV
    strcat(concat, "::");
    strcat(concat, "{0}");              // User (Optional)
    strcat(concat, "::");
    strcat(concat, (char *) master);    // Encryption Zone
    strcat(concat, "::");
    strcat(concat, encrypted_key.c_str());

    cout << "Writing key...";

    /* Expand our path, add the name as a folder and data dir */
    size_t path_size = strlen(path.c_str()) + strlen(name.c_str())
                       + strlen(combined.c_str()) + strlen(sub_path) + strlen(master) + 16;

    char key_path[sizeof(char) * path_size];
    strcpy(key_path, full_path.c_str());
    strcat(key_path, sub_path); // keystore/
    strcat(key_path, master);
    strcat(key_path, ".cey");

    FILE *f = fopen(key_path, "w+");
    if (f == NULL) {
        cout << "[ERROR] Opening file failed!" << endl;
        // TODO: Attempt to undo creation process
        return;
    }
    fprintf(f, concat);
    fclose(f);
    cout << "done" << endl;

    ////////////////////////////////////////////////////////////////////////////////////
    ///////////////////// Create a master config & write that to directory
    ////////////////////////////////////////////////////////////////////////////////////

    target /= "master.cfg";
    string config_path = target.c_str();
    wordfree(&expantion);

    /* Now write our default configuration to disk */
    Config cfg;

    /* Get root setting and add sub-settings */
    Setting &root = cfg.getRoot();

    auto time = std::chrono::system_clock::now();

    char *hostname = new char[128];
    gethostname(hostname, 128);

    char *username = new char[128];
    getlogin_r(username, 128);

    root.add("creation_date", Setting::TypeInt64) = time.time_since_epoch().count();
    root.add("last_updated", Setting::TypeInt64) = time.time_since_epoch().count();
    root.add("creation_machine", Setting::TypeString) = hostname;
    root.add("updating_machine", Setting::TypeString) = hostname;
    root.add("creation_user", Setting::TypeString) = username;
    root.add("updating_user", Setting::TypeString) = username;
    root.add("khash", Setting::TypeString) = this->pw_hash;

    cout << "Writing config to file..." << endl;

    /** Then write our configuration */
    cfg.writeFile(config_path.c_str());

    delete (hostname);
    delete (username);

    /* Log that we are done  */
    cout << "Done creating vault!" << endl;
}

ree_vault::ree_vault(rdb_uuid uuid, string path, string passphrase) {

}

ree_vault::~ree_vault() {

}

void ree_vault::close_vault() {
    cout << "\t> " << "Closing vault";
    cout.flush();

    // Do cleanup

    cout << "[DONE]" << endl;
}

int ree_vault::unlockVault(string passphrase) {

    cout << "About to check passphrase" << endl;

    char *to_consider = rcry_utils::md_sha256_salted(this->salt, passphrase.c_str(), true);

    if (strcmp(to_consider, this->pw_hash) == 0) {
        cout << "Password hash matches. Accepting token authentication..." << endl;
        return 0;
    } else {
        cout << "Password hash test failed. Token authentication rejected!" << endl;
        return 6;
    }
}

bool ree_vault::check_file_existance(string name) {
    return (*headers)[name] ? true : false;
}

void ree_vault::add_file(string name) {

    /* Create datafile and datafile header */
    filesystem::path target(this->path);
    target /= "/datastore/";

    datafile *d = new datafile(name, target.string());
    // datafile_h *dh = new datafile_h();

    /*  */
}

void ree_vault::update_file(string name, map<string, string> *content) {

}