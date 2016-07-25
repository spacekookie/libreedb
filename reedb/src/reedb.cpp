/* libreedb - reedb.c
 *
 * This file defines all the public core functions for libreedb. From
 * it you will be able to create management instances as well as
 * standalone vaults with the respective runtime and configuration flags.
 * For more information, please consult the official documentation or
 * the repository wiki.
 *
 * (c) 2016 					Lonely Robot.
 * Authors:						Katharina 'spacekookie' Sabel
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * -------------------------------------------
 *
 * A (hopefully) logical API :)
 */

/******** Include statements first ********/
#include <reedb/reedb.h>
#include <reedb/errors.h>
#include <reedb/platform.h>

#include "crypto/rcry_engine.h"

#include "protos/rdb_config.pb.h"
using reedb_proto::rdb_config;
using reedb_proto::rdb_config_vault;

#include <iostream>
using std::cout;
using std::endl;

#include <boost/filesystem.hpp>
using namespace boost;

#include <libconfig.h++>
using namespace libconfig;

#include <chrono>
#include <wordexp.h>
#include <stdlib.h>
#include <malloc.h>
#include <utils/uuid.h>


////////////////////////////////////////////////////
/////////////////                  ////////////////
//////////////// MANAGEMENT CLASS ////////////////
///////////////                  ////////////////
////////////////////////////////////////////////

#define REEDB_CONFIG "reedb.cfg"
#define REEDB_SCOPE_FILE "vault.scope"

/**
 * Small utility function that gets called when cleaning the scope file.
 *
 * Because vaults get added at the beginning of a transaction or because
 * the filessystem and user land might change it is possible to have
 * invalid vaults scoped in the file.
 *
 * Thus...every vault is tested for validity and if it isn't, removed!
 *
 * @param cfg protobuf structure for data
 */
void scrub_scope(rdb_config *cfg) {

}

void fill_config(const char *path) {

    /* Get root setting and add sub-settings */
    Config cfg;
    Setting &root = cfg.getRoot();

    auto time = std::chrono::system_clock::now();

    root.add("creation_date", Setting::TypeInt64) = time.time_since_epoch().count();
    root.add("last_modified", Setting::TypeInt64) = time.time_since_epoch().count();

    cfg.writeFile(path);
}

map<string, string> read_config(const char *path) {
    map<string, string> result;
    Config cfg;
    cfg.readFile(path);

    return result;
}

class rdb_cfg_wrapper {
public:
    reedb_proto::rdb_config  cfg;
};

/** Simple constructor */
rdb_core::rdb_core() {

    /** Create a handle for active vaults */
    this->active_vaults = new map<rdb_uuid*, rdb_vault*>();

    /** Create a list for crypto engines */
    this->r_cryptons = new list<rcry_engine*>();

    this->rdb_scope = new rdb_cfg_wrapper();

    /** Log regardless of verbosity because we can't know at this time */
    cout << "Launching new reedb instance..." << endl;
    std::flush(cout);

    /** Some stack variables to work with */
    bool created;
    string tmp_path;
    string tmp_log_path;

    /** First we need to find out our current operating system */
    long platform = rdb_platform::get_os();
    switch(platform) {
        case RDB_PLATFORM_LINUX:

            /** Check if we're running as root */
            if(getuid()) {
                tmp_path = "~/.config/reedb/";
                tmp_log_path = "~/.config/reedb/";
            } else {
                tmp_path = "/etc/reedb/";
                tmp_log_path = "/var/log/reedb/";
            }

            break;

        case RDB_PLATFORM_OSX:

            /** Check if we're running as root */
            if(getuid()) {
                tmp_path = "~/Library/Application Support/reedb/";
                tmp_log_path = "~/.config/reedb/";
            } else {
                throw config_error("It's not supported to run Reedb as root on Mac OS X...yet", NOT_IMPLEMENTED);
//                tmp_path = "/etc/reedb/";
//                tmp_log_path = "/var/log/reedb/";
            }
            break;

        default:
            throw gen_rdberror("Operating System " + std::to_string(platform) + " is not supported!", OS_NOT_SUPPORTED);
    }

    /** Create word expanders to get around "~" */
    wordexp_t tmp_path_exp, log_path_exp;
    wordexp(tmp_path.c_str(), &tmp_path_exp, 0);
    wordexp(tmp_log_path.c_str(), &log_path_exp, 0);

    /* Use boost to create the full path variables */
    filesystem::path path_trgt(tmp_path_exp.we_wordv[0]);
    filesystem::path log_trgt(log_path_exp.we_wordv[0]);

    /** Attempt to create both directories */
    created = filesystem::create_directories(path_trgt);
    path_trgt /= REEDB_CONFIG;

    if(created) {

        /** Create, fill and write a config file */
        fill_config(path_trgt.c_str());
    }

    /** Read current config from disk */
    map<string, string> cfg = read_config(path_trgt.c_str());
    path_trgt.remove_leaf();

    // TODO: Read in config that we just wrote in case we didn't just create it!

    /**************** Write/ Read Scope file ****************/

    path_trgt /= REEDB_SCOPE_FILE;

    cout << "Scope file is: " << path_trgt.string() << endl;
    filesystem::path scope_file(path_trgt.string());

    /** Get current hostname */
    string hostname = rdb_platform::get_machine();

    if(!filesystem::exists(scope_file) ) {

        /** Make a new default scope file */
        cout << "Scope file doesn't exists!" << endl;

        this->rdb_scope->cfg.set_machine(string(hostname));

        this->output = new fstream(path_trgt.string(), ios::out | ios::trunc | ios::binary);
        if(!this->rdb_scope->cfg.SerializeToOstream(output)) {
            throw config_error("Failed to write scope file: " + path_trgt.string(), FILE_PARSE_FAILED);
        }

        /* Flush the file to disk :) */
        this->output->flush();

    } else {

        /** Attempt to read the scope file that should exist :) */
        this->input = new fstream(path_trgt.string(), ios::in | ios::binary);
        if (!this->rdb_scope->cfg.ParseFromIstream(input)) {
            throw config_error("Unable to parse scope file " + path_trgt.string(), FILE_PARSE_FAILED);
        }

        const string scope_name = this->rdb_scope->cfg.machine();

        if(scope_name.compare(hostname) != 0) {
            cout << "[ERROR] Machine names " << hostname << " and " << scope_name << " don't match!" << endl;
            cout << "It is not recomended to move scope files between machines. "
                    << "Re-run with \"force\" to ignore this warning!" << endl;
        }

        int scoped = this->rdb_scope->cfg.scoped_size();
        cout << "Reading scope file successful. Found " << scoped << " vaults!" << endl;
    }

    /** Make sure we can log properly */
    filesystem::create_directories(log_trgt);

    /** Finally set the paths as reference */
    this->cfg_path = path_trgt.string();
    this->log_path = log_trgt.string();

    /********************************************************************************************/

    /** Create one initial crypto engine */
//    rcry_engine *def = new rcry_engine(0);
//    this->r_cryptons->push_back(def);

    cout << "Reedb initialisation complete!" << endl;
}

/** Cleans up all resources */
rdb_core::~rdb_core() {
    if(input) delete(input);
    if(output) delete(output);

    google::protobuf::ShutdownProtobufLibrary();

    delete(this->r_cryptons);
    delete(this->active_vaults);
    delete(this->rdb_scope);
}

/** Creates a new vault object to play with */
rdb_vault* rdb_core::create_vault(string name, string path) {
    rdb_uuid *uuid = uuid_helper::generate();
    rdb_vault *vault = new rdb_vault(name, path);

    /* Scope it now, we might not get a chance later */
    {
        vault_meta meta;
        meta.size = 0;
        meta.id = rdb_uuid(*uuid);
        meta.active = false;
        meta.name = name;
        meta.path = path;

        /* Call scope with our meta */
        this->scope_vault(&meta);
    }

    /* Return the vault for user manipulation */
    return vault;
}

list<vault_meta> rdb_core::list_vaults() {
    list<vault_meta> vaults;

    /** Get an auto ptr and iterate over collction */
    auto av = this->rdb_scope->cfg.scoped();
    for(rdb_config_vault &v : av) {
        vault_meta m;

        /** Get data out of proto structure */
        string name = v.name();
        string path = v.path();
        int64_t size = v.size();

        rdb_uuid id;
        string uuid = v.uuid();
        memcpy((unsigned char*) id.x, uuid.c_str(), 32);

        bool active = (*this->active_vaults)[&id] == NULL;

        /** Now we have all the info we need. So we make a list **/
        m.name = name;
        m.path = path;
        m.active = active;
        m.id = rdb_uuid(id);
        m.size = size;

        vaults.push_back(m);
    }

    return vaults;
}

void rdb_core::scope_vault(vault_meta *meta) {
    rdb_config_vault *v = this->rdb_scope->cfg.add_scoped();
    v->set_name(meta->name);
    v->set_path(meta->path);

    unsigned char *uuid = (unsigned char*) malloc(sizeof(unsigned char) * sizeof(meta->id.x));
    memcpy(uuid, meta->id.x, sizeof(meta->id.x));
    string buf_uuid = string((char*) uuid);
    v->set_uuid(buf_uuid);
    delete(uuid);

    v->set_size(meta->size);
}

void rdb_core::unscope_vault(rdb_uuid *id) {
    // Convert our fixed array into a C++ string. Why do we even bother?
    cout << "Preparing to unscope vault with id: " << id->x << endl;
    auto scoped = this->rdb_scope->cfg.mutable_scoped();

    /** Iterate over our mutable record set and find the vault */
    for(int i = 0; i < scoped->size(); i++) {
        auto &v = scoped->Get(i);
        string _uuid = v.uuid();

        /** Copy the uuid from C++ string to more usable format */
        unsigned char uuid[32];
        memcpy(uuid, _uuid.c_str(), 32 * sizeof(unsigned char));

        // FIXME: Put this in a function or something.
        // Not sure why normal string compares don't work on the UUID but hey!
        bool hit = true;
        {
            for(int a = 0; a < 32 ; a++) {
                char f = uuid[a];
                char g = id->x[a];
                if(f != g) {
                    hit = false;
                    break;
                }
            }
        }

        if(hit) {
            scoped->DeleteSubrange(i, i + 1);
            cout << "Removed vault " << i << " from scope..." << endl;
            return;
        }
    }
}

////////////////////////////////////////////////////
/////////////////                  ////////////////
////////////////    VAULT CLASS   ////////////////
///////////////                  ////////////////
////////////////////////////////////////////////

rdb_vault::rdb_vault(string name, string path) {
    cout << "Preparing new vault for future transactions" << endl;
}

rdb_vault::~rdb_vault() {

}