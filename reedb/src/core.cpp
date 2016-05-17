/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

// Public facing reedb includes
#include "reedb/core.h"

// Runtime dependencies
#include <iostream>
#include <vector>
#include <map>

#include <malloc.h>

// Internal system dependencies
#include <reedb/vaults.h>
#include "crypto/rcry_engine.h"
#include "crypto/rcry_context.h"

using namespace std;

reedb::reedb() {
    this->finalised = false;
    if (RDB_DEBUG) cout << "Preparing to start a new Reedb instance...";

    /* Populate sane defaults */
    this->daemon = false;
    this->verbose = true;
}

reedb::~reedb() {
    if (RDB_DEBUG) cout << "=== Running cleanup ===" << endl;

    for (rdb_vaults *interface : this->vault_interfaces) {
        delete (interface);
    }
}

bool reedb::isReady() {
    return finalised;
}

void reedb::register_vinterface(rdb_vaults *interface) {
    string interface_id = interface->get_id();
    for (rdb_vaults *rv : this->vault_interfaces) {
        if (rv->get_id() == interface_id) {
            if (RDB_DEBUG) if (RDB_DEBUG) cout << "[ERROR] This interface is already registered! Skipping..." << endl;
            return;
        }
    }
    if (this->verbose) if (RDB_DEBUG) cout << "Adding interface with id " << interface_id << " to reedb instance..." << endl;
    this->vault_interfaces.push_back(interface);
}

void reedb::finalise() {
    if (RDB_DEBUG) if (RDB_DEBUG) cout << "Checking user settings and starting Reedb" << endl;

    /* Make sure that the singleton context exists  */
    rcry_context *context = rcry_context::instance();
    int count = context->get_count_incr();

    #if verbose
    #define RDB_DEBUG true
    #endif

    rcry_engine **engines = (rcry_engine **) malloc(sizeof(rcry_engine *) * 4);

    /* Create an engine and actually save it */
    rcry_engine *e = new rcry_engine(count);
    engines[count] = e;

    /* Transfer our pointer-pointer into the context */
    context->rcry_context_update(engines);

    /* From now on it's ready to be used */
    this->finalised = true;
}

void reedb::terminate() {
    if (RDB_DEBUG) cout << "Instance " << this << " is being terminated. All active vaults_interface to be closed..." << endl;
    delete (this);
}
