/* Internal Reedb requirements */
#include "datafile.h"

/* Some system requirements for data storage and file access*/
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "protos/rdb_data.pb.h"
#include "crypto/rcry_utils.h"

extern "C"
{
#include "utils/files.h"
}

// C runtimes
#include <malloc.h>
#include <string.h>

// Boost serialisation includes (TO BE DELETED)
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <crypto/rcry_token.h>

using namespace std;
using namespace reedb_proto;

datafile::datafile(string name, string parent) {

    /* Asign data to fields */
    this->data = new rdb_data();
    this->data->set_name(name);

    // FIXME Is this still required?
    this->name = name;

    char *buffer = rcry_utils::md_sha256(name.c_str(), true);
    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = tolower(buffer[i]);
    }

    this->path = parent + buffer + ".rdf"; // Reedb datafile
    this->version = 1;

    free(buffer);

    if (RDB_DEBUG) cout << "Just created a new datafile" << endl;

    /* Then populate them */
    this->populate();

    /* Set cache mode to hotdrop so we can use the file immediately */
    this->cache(HOTDROP);
}

datafile::datafile(string name, reedb_proto::rdb_data *old_file) {
    if (RDB_DEBUG) cout << "Preparing an existing datafile for transactions..." << endl;
}

void datafile::populate() {
    data->set_name(this->name);
    data->set_category("SOMETHING");

    //    rdb_data::revision *revision = data->add_revs();
    //    revision->set_rev_no(0);
    //
    //    /** Create a string key-value pair in our revision */
    //    rdb_data::string_pair *pair = revision->add_sentry();
    //    pair->set_key("Sample");
    //    pair->set_val("This is some sample data");
}

void datafile::write(rcry_engine *engine, rcry_token *token) {
    if (RDB_DEBUG) cout << "Writing datafile" << endl;

    /* First we have to make the data usable to us */
    string data = serialise(this->data);

    /** Go and encrypt the data */
    engine->start_batch(token, false);
    char *encrypted = engine->encrypt((void*) data.c_str(), token);
    engine->end_batch(token);

    /** Then dump the data to disk */
    rdb_files_dfdump(this->path.c_str(), encrypted);
}

map<string, string> *datafile::read(rcry_engine *engine, rcry_token *token)
{
    if (RDB_DEBUG) cout << "Reading from datafile..." << endl;

    if(!this->data)
    {
        if (RDB_DEBUG) cout << "\t" << "Cache empty: loading from backing storage..." << endl;
        char *raw;
        rdb_files_dfread(this->path.c_str(), raw);

        /** Decrypt the data */
        engine->start_batch(token, false);
        char *encoded = engine->decrypt(raw, token);

        /** Then deserialise the data */
        this->data = deserialise(encoded);
    }

    /** Transpose protobuf to C++ std map */
    map<string, string> *carry = new map<string, string>();

    /** Iterate over revisions in proto ... */
    for(auto &iter : this->data->revs())
    {
        /** For every string entry in said revision ... */
        for(auto &entry : iter.sentry()) (*carry)[entry.key()] = entry.val();
    }

    return carry;
}

/** Loads in data from an open file to the header */
void datafile::populate_header(datafile_h *dh) {
    dh = new datafile_h();

    dh->category = this->data->category();
    dh->name = new string(this->name);
    dh->tags = (string*) malloc(sizeof(std::string*));
    dh->version = this->version;
    dh->locked = false;
}

void datafile::close() {
    delete this->data;
}

void datafile::insertData(reedb_proto::rdb_data::revision *rev, string key, string val) {
    if(this->data)
    {
        rdb_data::string_pair *pair = rev->add_sentry();
        pair->set_key(key);
        pair->set_val(val);
    }
}

reedb_proto::rdb_data::revision* datafile::incr_revision() {
    if(this->data)
    {
        rdb_data::revision *r = this->data->add_revs();
        int revisions = this->data->revs_size();
        r->set_rev_no(revisions);
        return r;
    } else {
        if (RDB_DEBUG) cout << "[ERROR] Unencrypted datafile cache not available!" << endl;
    }
}

void datafile::cache(const cache_mode mode) {

}

string datafile::serialise(rdb_data *proto) {
    if (RDB_DEBUG) cout << "Serialising datafile...";
    string s = proto->SerializeAsString();

    if (RDB_DEBUG) cout << "done" << endl;
    return s;
}

rdb_data *datafile::deserialise(string data) {
    if (RDB_DEBUG) cout << "Deserialising datafile...";
    rdb_data *d = new rdb_data();
    d->ParseFromString(data);

    if (RDB_DEBUG) cout << "done" << endl;
    return d;
}