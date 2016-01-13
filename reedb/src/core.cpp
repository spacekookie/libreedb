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
#include "reedb/crypto/token.h"
#include "reedb/utils/helper.h"
#include "reedb/utils/uuid.h"
#include "reedb/core.h"

//
#include <iostream>
#include <map>

using namespace std;

reedb::reedb()
{
  cout << "Preparing to start a new Reedb instance..." << endl;
  
  if(verbose) cout << "Defining sane defaults...";
  
  /* Populate sane defaults */
  this->daemon = false;
  this->verbose = true;
  
  /* Some verbose loging */
  if(verbose) cout << "done" << endl;
  if(verbose) cout << "Waiting for finialisation..." << endl;
}

reedb::~reedb()
{
  cout << "=== Running cleanup ===" << endl;
  
  for(rdb_vaults* interface : this->vault_interfaces) {
    delete(interface);
  }
}

bool reedb::isReady()
{
  return finalised;
}

void reedb::register_vinterface(rdb_vaults* interface)
{
  string interface_id = interface->get_id();
  for(rdb_vaults* rv : this->vault_interfaces) {
    if(rv->get_id() == interface_id) {
      cout << "[ERROR] This interface is already registered! Skipping..." << endl;
      return;
    }
  }
  if(this->verbose) cout << "Adding interface with id " << interface_id << " to reedb instance..." << endl;
  this->vault_interfaces.push_back(interface);
}

void reedb::finalise()
{
  if(verbose) cout << "Checking user settings and starting Reedb" << endl;

  /* From now on it's ready to be used */
  this->finalised = true;
  
  rdb_token *token = rdb_tokens_create(1);
  
  std::cout << "Our token: " << token->contents << std::endl;
}

void reedb::terminate()
{
  cout << "Instance " << this << " is being terminated. All active vaults are closed and dropped..." << endl;
  
  delete(this);
}
