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

#include <iostream>
#include <reedb/core.h>
#include <reedb/vaults.h>

using namespace std;

#include <gcrypt.h>

int main(int argc, char **args) {

    reedb *rdb = new reedb();
    rdb->set_os(LINUX);
    rdb->set_distro(FEDORA);
    rdb->set_verbose(true);
    rdb->finalise();

    rdb_vaults *v = new rdb_vaults();
    rdb->register_vinterface(v);

    /* Create a vault and then query its state */
    vault_meta *vault = v->create("default", "~/Documents/", "foobar32");

    cout << "Name: " << *vault->name << endl;
    cout << "ID: " << vault->id->id << endl;
    cout << "Size: " << vault->size << endl;
    cout << "Path: " << *vault->path << endl;
    cout << "Active: " << vault->active << endl;

    rdb->terminate();
    return 0;
}
