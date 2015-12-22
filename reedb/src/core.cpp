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

#include "reedb/core.h"
#include <iostream>

using namespace std;

Reedb::Reedb()
{
  cout << "Preparing to start a new Reedb instance..." << endl;
  
  /* Populate sane defaults */
  this->passLength = 12;
  this->vaultCount = 0;
  this->daemon = false;
  
  /* Development defaults... TODO: REMOVE THESE */
  this->os = LINUX;
  this->verbose = true;
  this->distro = FEDORA;
  
  if(verbose) cout << "Defining sane defaults...done" << endl;
  if(verbose) cout << "Waiting for finialisation...";
}

void Reedb::finalise()
{
  if(verbose) cout << "done" << endl;
  if(verbose) cout << "Checking user settings and starting Reedb" << endl;
  
  if(this->passLength < 6) cout << "[WARN] Minimum passphrase length is realistically too low! Security may be severely compromised!" << endl;

  
  
}
