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
using namespace std;

// Main reedb header for interfaces
#include <reedb/reedb.h>

// Optional header for easier error handling
#include <reedb/errors.h>

int main(void) {

    try {
        /* Initialise the context */
        rdb_core *core = new rdb_core();

        /* Create a new vault to play with */
        rdb_vault *vault = core->create_vault("my_vault", "~/Documents/");

        /** For sanity check get a list of vaults */
        list<vault_meta> vaults = core->list_vaults();
        cout << vaults.size() << " vaults found!" << endl;

        vault_meta *meta;
        for(auto &iter : vaults) {
            meta = &iter;
            cout << "=== Vault ===" << endl;
            cout << "Name: '" << iter.name << "'" << endl;
            cout << "Path: '" << iter.path << "'" << endl;
            cout << "Size: " << iter.size << endl;
        }

        core->unscope_vault(&meta->id);

        /** Then check if it worked */
        list<vault_meta> vaults2 = core->list_vaults();
        cout << vaults2.size() << " vaults found!" << endl;

        delete(vault);
        delete(core);

    } catch (reedb_error &e) {
        cout << endl;           // Create a blank line before the error msg

        cout << "An Exception occured with code: " << e.get_code() << endl;
        cout << "=> " << "Error Message: '" << e.what() << "'" << endl;

        return e.get_code();    // Quit our application with this code
    }

    cout << "Test run: COMPLETE!" << endl;
    return 0;

}


//    core->set_verbose(false);

//    rdb_vault *my_vault = core->create_vault("my_vault", "~/Documents/");
//
//    /** Setting up our vault with some parameters
//     *
//     * - MANAGED                Multi User with Super Admin (root)
//     * - LOG_VERBOSE            Override the non-verbose logging from the instance management
//     * - CACHE_QUICK            Focus on access speed, not security
//     * - CACHE_BUFFERED         Hand off write operations to make insertions faster
//     * - WRITE_TREE             Don't create a block device but a classic file tree
//     * - FTR_SINGLE_RECORD      Enable the "Single record" feature. Nothing else is allowed
//     * - FTR_DISABLE_LUT        Disables the header lookup table. Will make the vault *very* slow!
//     * - FTR_PERMISSIVE         Ignore certain token authentication errors (not recomended for production!)
//     * - FTR_DISABLE_HEADERS    Don't keep headers in records, only data bodies.
//     */
//    my_vault->set_flags(RDB_FLG_MANAGED |
//                        RDB_FLG_LOG_VERBOSE |
//                        RDB_FLG_CACHE_QUICK |
//                        RDB_FLG_CACHE_BUFFERED |
//                        RDB_FLG_WRITE_TREE |
//                        RDB_FLG_FTR_SINGLE_RECORD |
//                        RDB_FLG_FTR_DISABLE_LUT |
//                        RDB_FLG_FTR_PERMISSIVE |
//                        RDB_FLG_FTR_DISABLE_HEADERS);
//
//    /**
//     * We chose a managed vault so we need to define our root user login details
//     */
//    my_vault->set_login(RDB_USER_ROOT, "reedb_is_awesome_and_secure:)");
//
//    /** For the sake of this demonstration, let's add another zone and a user for it */
//    long cool_zone_id = my_vault->add_zone("Cool Zone");
//
//    /** Just like with flags, you can combine zone IDs */
//    long spacekookie_id = my_vault->add_user("spacekookie", cool_zone_id | RDB_ZONE_ROOT);
//    my_vault->set_login(spacekookie_id, "MargeretThatcheris110%sexy");

    /**
     * Then we need to call finish setup to create the vault. Recardless to say this would be a
     * _terrible_ vault setup. But libreedb lets you do it. Because you're the boss.
     *
     * Default values are somewhat sane so if you don't want to fiddle with it, you can just leave
     * them for a balance between security and speed.
     *
     * Please consult the documentation for more in-depth explanation about the configuration
     * flags and settings that you might want to avoid in production systems.
     */
    // my_vault->finish_setup();


//
//int main(void) {
//
//    // First create our reedb core
//    reedb *rdb = new reedb();
//    rdb->set_os(LINUX);
//    rdb->set_distro(FEDORA);
//    rdb->set_verbose(false);
//    rdb->finalise();
//
//    // Then attach a new vaults interface to it
//    rdb_vaults *v = new rdb_vaults();
//    rdb->register_vinterface(v);
//
//    /* Create a vault and then query its state */
//    const string passphrase = "foobar32";
//    vault_meta *vault = v->create("default", "~/Documents/", passphrase);
//    rdb_token token = v->authenticate(vault->id, passphrase);
//
//    string filename = "Example";
//    map<string, string> data = map<string, string>();
//    data["Field A"] = "foobar32"; // Ironically saving the vault pw in itself
//    data["Field B"] = "My Username";
//    v->insert(vault->id, &token, filename, &data);
//
//    map<string, string> *recovered = v->query_file(vault->id, &token, filename);
//
//    cout << "========== Recovered Data ==========" << endl;
//    for(auto &iter : *recovered)
//    {
//        cout << "[" << iter.first << "] -> [" << iter.second << "]" << endl;
//    }
//
//    cout << "=====================================" << endl;
//
//    /** Close the vault to force a sync TODO: Add a function for that */
//    v->close(vault->id, &token);
//
//    /** Open vault again */
//    token = v->authenticate(vault->id, passphrase);
//    map<string, string> *recovered_two = v->query_file(vault->id, &token, filename);
//
//    cout << "========== Recovered Data 2 ==========" << endl;
//    for(auto &iter : *recovered_two)
//    {
//        cout << "[" << iter.first << "] -> [" << iter.second << "]" << endl;
//    }
//
//    cout << "======================================" << endl;
//
//    // Just exit everything
//    v->close(vault->id, &token);
//    rdb->terminate();
//}