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

//#include "reedb/core.h"
//#include "reedb/vaults.h"
//
//#include <map>
//#include <string>

#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"

using namespace tutorial;
using namespace std;

// This function fills in a Person message based on user input.
void PromptForAddress(tutorial::Person* person) {
    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');

    cout << "Enter name: ";
    getline(cin, *person->mutable_name());

    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty()) {
        person->set_email(email);
    }

    while (true) {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty()) {
            break;
        }

        tutorial::Person::PhoneNumber* phone_number = person->add_phone();
        phone_number->set_number(number);

        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile") {
            phone_number->set_type(tutorial::Person::MOBILE);
        } else if (type == "home") {
            phone_number->set_type(tutorial::Person::HOME);
        } else if (type == "work") {
            phone_number->set_type(tutorial::Person::WORK);
        } else {
            cout << "Unknown phone type.  Using default." << endl;
        }
    }
}

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 2) {
        cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
        return -1;
    }

    AddressBook ab;
    Person *p = ab.add_person();
    p->set_id(0);
    p->set_name("Katharina Sabel");
    p->set_email("katharina.sabel@mailbox.org");
    Person::PhoneNumber *p_phone = p->add_phone();
    p_phone->set_number("+4917670422074");
    p_phone->set_type(Person_PhoneType_MOBILE);

    {
        // Read the existing address book.
        fstream input(argv[1], ios::in | ios::binary);
        if (!input) {
            cout << argv[1] << ": File not found.  Creating a new file." << endl;
        } else if (!ab.ParseFromIstream(&input)) {
            cerr << "Failed to parse address book." << endl;
            return -1;
        }
    }

    // Add an address.
    // PromptForAddress(address_book.add_person());

    {
        // Write the new address book back to disk.
        fstream output(argv[1], ios::out | ios::trunc | ios::binary);
        if (!ab.SerializeToOstream(&output)) {
            cerr << "Failed to write address book." << endl;
            return -1;
        }
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

//    // First create our reedb core
//    reedb *rdb = new reedb();
//    rdb->set_os(LINUX);
//    rdb->set_distro(FEDORA);
//    rdb->set_verbose(true);
//    rdb->finalise();
//
//    // Then attach a new vaults interface to it
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
//    // Just exit everything
//    rdb->terminate();
// }
