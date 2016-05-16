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

    cout << "File path: " << this->path << endl;
    cout << "Just created a new datafile" << endl;

    /* Then populate them */
    this->populate();

    /* Set cache mode to hotdrop so we can use the file immediately */
    this->cache(HOTDROP);
}

datafile::datafile(string name, reedb_proto::rdb_data *old_file) {
    cout << "Preparing an existing datafile for transactions..." << endl;
}

void datafile::populate() {
    cout << "Populating datafile...";

    data->set_name(this->name);
    data->set_category("SOMETHING");

    rdb_data::revision *revision = data->add_revs();
    revision->set_rev_no(0);

    /** Create a string key-value pair in our revision */
    rdb_data::string_pair *pair = revision->add_sentry();
    pair->set_key("Sample");
    pair->set_val("This is some sample data");

    cout << "done" << endl;
}

void datafile::write(rcry_engine *engine, rcry_token *token)
{

    cout << "Writing datafile with token: " << token->contents << endl;

    /* First we have to make the data usable to us */
    void *data = serialise(this->data);

    cout << "Data we print: " << (char*) data << endl;

    /** Go and encrypt the data */
    engine->start_batch(token, false);
    char *encrypted = engine->encrypt(data, token);
    engine->end_batch(token);

    /** Then dump the data to disk */
    rdb_files_dfdump(this->path.c_str(), encrypted);
}

map<string, string> *datafile::read(rcry_engine *engine, rcry_token *token)
{

}

void datafile::cache(const cache_mode mode) {

}

void *datafile::serialise(rdb_data *proto)
{
    cout << "Serialising datafile...";

    /** Get the size and store it */
    int size = proto->ByteSize();

    /** Allocate the neccessary memory and write our proto into it */
    void *buffer = malloc(size + sizeof(int));
    proto->SerializeToArray(buffer + (sizeof(int)), size);

    /** Now write the size to the front so we can get our proto back later */
    ((int*) buffer)[0] = size;

    cout << "done" << endl;
    return buffer;
}

rdb_data *datafile::deserialise(void *data) {
    int size = ((int*) data)[0];

//    this->data = new rdb_data();
//    this->data->ParseFromArray(data + sizeof(int), size);

//    map<string, string> new_map;
//    stringstream ss;
//    ss.str(data);
//
//    boost::archive::text_iarchive iarch(ss);
//    iarch >> new_map;
//    return new_map;
}