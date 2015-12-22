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

/* Internal reedb dependencies */
#include "reedb/utils/helper.h"

using namespace std;

class Reedb
{
public:
  
  /* Public constructor, destructor and equals operator overloading */
  Reedb();
  ~Reedb();
  bool operator==(const Reedb& other) const;
  
  /* Manually override the minimum required passphrase length for a vault */
  void setPassLength(unsigned int length);
  
  /** Provide a custom user function to execute concurrent to the Reedb threads */
  void setUsrcode(void *function);
  
  /** Make Reedb run in daemon mode. This detaches all logging to file and makes the runtime more autonomous. */
  void setDaemon(bool daemon);
  
  /** Set reedb verbose debugging mode. WARNING: Everything will be logged! */
  void setVerbose(bool verbose);
  
  /** Set the operational path of Reedb. This is automatically set by default */
  void setPath(string path);
  
  /* Manually set the OS */
  void setOs(ree_os os);
  
  /* Manually set the distribution */
  void setDistro(ree_distro distro);
  
  /** Toggle a force override to ignore previous instances of Reedb. WARNING: Can cause
   * damage to data and corrupt an open vault!
   */
  void setOverride(bool override);
  
  /* Finalise creation and init Reedb */
  void finalise();
  
  /* Terminate the current instance of reedb gracefully */
  void terminate();
  
private:
  
  /* Basic info about the vault */
  string *name;
  string *path;
  
  /* File count in the vault storage and minimum passphrase length */
  unsigned int vaultCount;
  unsigned int passLength;
  
  /* Store the current OS and distribution for file paths and permissions */
  ree_os os;
  ree_distro distro;
  
  /* Is reedb running in detached daemon mode? */
  bool daemon;
  
  /* Is reedb running in verbose logging mode? */
  bool verbose;
};

#endif // REEDB_H
