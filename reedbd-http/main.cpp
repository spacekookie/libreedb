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

#include <reedb/core.h>
#include <reedb/vaults.h>
#include <iostream>
#include <vector>
#include <gcrypt.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <unistd.h>

using namespace std;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HexEncoder;

int main(int argc, char **args) {

    int length = 1024;
    int count = 0;

    for (int c = 0; c <= 1024; c++) {
        char *A = (char *) gcry_random_bytes(length, GCRY_VERY_STRONG_RANDOM);
//        char *B = (char *) gcry_random_bytes(length, GCRY_VERY_STRONG_RANDOM);
//        char *C = (char *) gcry_random_bytes(length, GCRY_VERY_STRONG_RANDOM);
//        char *D = (char *) gcry_random_bytes(length, GCRY_VERY_STRONG_RANDOM);
//        char *E = (char *) gcry_random_bytes(length, GCRY_VERY_STRONG_RANDOM);

        string enA, enB, enC, enD, enE;
        StringSource((byte *) A, length, true, new HexEncoder(new StringSink(enA)));
//        StringSource((byte *) B, length, true, new HexEncoder(new StringSink(enB)));
//        StringSource((byte *) C, length, true, new HexEncoder(new StringSink(enC)));
//        StringSource((byte *) D, length, true, new HexEncoder(new StringSink(enD)));
//        StringSource((byte *) E, length, true, new HexEncoder(new StringSink(enE)));
        cout << "Secure: " << enA << endl; // "-" << enB << "-" << enC << "-" << enD << "-" << enE << endl;

        free(A);
//        free(B);
//        free(C);
//        free(D);
//        free(E);

        unsigned int microseconds = 50000;
        usleep(microseconds);
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
//    v->create("default", "~/Documents/", "foobar32");
//    vector<vault_head> *vaults = v->list_vaults();
//
//    rdb_uuid *uuid;
//    rdb_token token;

//    for (vault_head vh : *vaults) {
//        cout << endl << "=== " << *(vh.name) << " ===" << endl;
//        cout << "ID: " << (vh.id) << endl;
//        cout << "Path: " << *(vh.path) << endl;
//        cout << "Active: " << vh.active << endl;
//        cout << "Size: " << vh.size << endl;
//
//        //        cout << "Trying to athenticate now!" << endl;
//        //        token = v->authenticate((vh.id), "foobar32", false);
//
//        //        if (token) {
//        //            cout << "Authentication FAILED!" << endl;
//        //            return 255;
//        //        }
//    }

    /* Shut it down */
//    rdb->terminate();
//
//    std::cout << "We're done here..." << std::endl;
//    return 0;
}
