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

#ifndef REEDB_H
#define REEDB_H

#include <string>
#include <map>

/* Some utilities */
#include "reedb/utils/helper.h"
#include "reedb/utils/uuid.h"

using namespace std;

class reedb
{
public:
  
  /* Public constructor, destructor and equals operator overloading */
  reedb();
  bool operator==(const reedb& other) const;

  /* Finalise creation and init reedb */
  void finalise();
  
  /* Terminate the current instance of reedb gracefully */
  void terminate();
  
  /* Return if this instance is ready to use */
  bool isReady();

private:
  
  /* Keep the destructor private so we are in control over resources */
  ~reedb();
  
  /* Where is the master config located */
  string *path;

  /* How many vaults are managed by this instance */
  unsigned int vaultCount;

  /* Publicly set parameters for initialisation*/
  SETTER(unsigned int, passLength)
  SETTER(ree_os, os)
  SETTER(ree_distro, distro)
  SETTER(bool, daemon)
  SETTER(bool, verbose)
  SETTER(bool, override)
  
  /* Indicate whether or not this instance of reedb is ready to be used */
  bool finalised = false;
  
  /* Map of scoped vaults for quick lookup */
  map<rdb_uuid, void*> *scoped;
};

#endif // REEDB_H
