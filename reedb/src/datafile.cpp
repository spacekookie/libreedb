/* Internal Reedb requirements */
#include "datafile.h"

/* Some system requirements for data storage and file access*/
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "crypto/rcry_utils.h"
#include "protos/rdb_data.pb.h"

// C runtimes
#include <malloc.h>
#include <string.h>

// Boost serialisation includes (TO BE DELETED)
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

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

    std::cout << "File dump complete. Ready to cache it from disk..." << endl;
}

void datafile::populate() {
    rdb_data::revision *revision = this->data->add_revs();
    revision->set_rev_no(0);

    /** Create a string key-value pair in our revision */
    rdb_data::string_pair *pair = revision->add_sentry();
    pair->set_key("Sample");
    pair->set_val("This is some sample data");
}

void datafile::write() {

    /* First we have to make the data usable to us */

    
}

void datafile::cache(const cache_mode mode) {

}

map<int, int> deserialise(string s) {

}

string datafile::serialise(map<string, string> data) {
    stringstream ss;
    boost::archive::text_oarchive oarch(ss);
    oarch << data;
    return ss.str();
}

map<string, string> datafile::deserialise(string data) {
    map<string, string> new_map;
    stringstream ss;
    ss.str(data);

    boost::archive::text_iarchive iarch(ss);
    iarch >> new_map;
    return new_map;
}