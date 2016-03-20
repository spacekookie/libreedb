/* Internal Reedb requirements */
#include "datafile.h"

/* Some system requirements for data storage and file access*/
#include <iostream>
#include <vector>
#include <map>

#include <string.h> // C string support
#include <string> // C++ strings

#include <crypto/rcry_utils.h>

using namespace std;

datafile::datafile(string name, string parent) {

    /* Asign data to fields */
    this->name = name;

    char *buffer = rcry_utils::md_sha256(name.c_str(), true);
    for (int i = 0; buffer[i] != '\0'; i++) {
        buffer[i] = tolower(buffer[i]);
    }

    this->path = parent + buffer + ".rdf"; // Reedb datafile
    this->version = 1;

    free(buffer);

    cout << "File path: " << this->path << endl;

    std::cout << "Just created a new datafile" << endl;

    /* Then populate them */
    this->populate();

    /* Set cache mode to hotdrop so we can use the file immediately */
    this->cache(HOTDROP);

    std::cout << "File dump complete. Ready to cache it from disk..." << endl;
}

void datafile::populate() {
    this->data = new vector<map<string, void *> *>;
}

void datafile::write() {

    /* First we have to make the data usable to us */
}

void datafile::cache(const cache_mode mode) {

}
