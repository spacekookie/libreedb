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

#include <reedb/vaults.h>
#include <reedb/core.h>
#include <iostream>

using namespace std;

int main(void) {

    // First create our reedb core
    reedb *rdb = new reedb();
    rdb->set_os(LINUX);
    rdb->set_distro(FEDORA);
    rdb->set_verbose(false);
    rdb->finalise();

    // Then attach a new vaults interface to it
    rdb_vaults *v = new rdb_vaults();
    rdb->register_vinterface(v);

    /* Create a vault and then query its state */
    const string passphrase = "foobar32";
    vault_meta *vault = v->create("default", "~/Documents/", passphrase);
    rdb_token token = v->authenticate(vault->id, passphrase);

    string filename = "Example";
    map<string, string> data = map<string, string>();
    data["Field A"] = "foobar32"; // Ironically saving the vault pw in itself
    data["Field B"] = "My Username";
    v->insert(vault->id, &token, filename, &data);

    map<string, string> *recovered = v->query_file(vault->id, &token, filename);

    cout << "========== Recovered Data ==========" << endl;
    for(auto &iter : *recovered)
    {
        cout << "[" << iter.first << "] -> [" << iter.second << "]" << endl;
    }

    cout << "=====================================" << endl;

    /** Close the vault to force a sync TODO: Add a function for that */
    v->close(vault->id, &token);

    /** Open vault again */
    token = v->authenticate(vault->id, passphrase);
    map<string, string> *recovered_two = v->query_file(vault->id, &token, filename);

    cout << "========== Recovered Data 2 ==========" << endl;
    for(auto &iter : *recovered_two)
    {
        cout << "[" << iter.first << "] -> [" << iter.second << "]" << endl;
    }

    cout << "======================================" << endl;

    // Just exit everything
    v->close(vault->id, &token);
    rdb->terminate();
}