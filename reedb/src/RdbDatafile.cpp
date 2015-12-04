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

#include "RdbDatafile.h"
#include <malloc.h>

RdbDatafile::RdbDatafile(string name, RdbVault *vault)
{
  /* Asign data to fields */
  this->name = name;
  this->path = vault->getPath() + "/" + vault->getName() + "/datastore/" + name;
  this->version = 1;
  
  /* Then populate them */
  this->populate();
  
  /* Set cache mode to hotdrop so we can use the file immediately */
  this->cache(HOTDROP);
  
  this->write();
}

void RdbDatafile::populate()
{
  /* We malloc enough space for two map pointers. One for us, one for the user */
  this->data = (map_t*) new map_t*[2];
  // this->data[0] = hashmap_new();
}
