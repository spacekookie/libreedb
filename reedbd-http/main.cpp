/* reedbd-http - main.c
 *
 * This file is the entry point to the Reedb HTTP(s) extention.
 * It is statically compiled against Reedb and adds functions for
 * web applications and usability without using the C bound API.
 *
 * (c) 2015 					Lonely Robot.
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
 * Initialisation of reedbd-http is done automatically (mostly).
 */

#include <map>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>

#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <reedb/utils/prettyprint.h>

using namespace std;

string serialise(map<int, int> m) {
    stringstream ss;
    boost::archive::text_oarchive oarch(ss);
    oarch << m;
    return ss.str();
}

map<int, int> deserialise(string s) {
    map<int, int> new_map;
    stringstream ss;
    ss.str(s);

    boost::archive::text_iarchive iarch(ss);
    iarch >> new_map;
    return new_map;
}

void write(const char *data, const char *filename) {
    FILE *fp;
    fp = fopen(filename, "w+");
    fprintf(fp, data);
    fclose(fp);
}

char *read(const char *filename) {
    char ch;
    int fd = open(filename, O_RDONLY);
    int len = lseek(fd, 0, SEEK_END);

    /* Read data via management memory space */
    char *data = (char *) mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
    return data;
}

#define DEMONAME "/home/spacekookie/file.rdf"

int main() {
    std::map<int, int> foo = {{1, 2},
                              {2, 1}};

    string buffer = serialise(foo);
    cout << "Buffer: " << buffer << endl;
    write(buffer.c_str(), DEMONAME);

    // Idle

    char *recovered = read(DEMONAME);
    cout << "Recovered: " << recovered << endl;

    map<int, int> ret = deserialise(string(recovered));
    cout << ret << endl;

    std::cout << (foo == ret) << std::endl;
}

//    reedb *rdb = new reedb();
//    rdb->set_os(LINUX);
//    rdb->set_distro(FEDORA);
//    rdb->set_verbose(true);
//    rdb->finalise();
//
//    rdb_vaults *v = new rdb_vaults();
//    rdb->register_vinterface(v);
//
//    /* Create a vault and then query its state */
//    string passphrase = "foobar32";
//    vault_meta *vault = v->create("default", "~/Documents/", passphrase);
//    rdb_token token = v->authenticate(vault->id, passphrase);
//
//    map<string, string> data = map<string, string>();
//    data["Field A"] = "foobar32"; // Ironically saving the vault pw in itself
//    data["Field B"] = "My Username";
//    v->insert(vault->id, &token, "Example", &data);
//
//    rdb->terminate();
