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
#include "reedb/utils/hashmap.h"

#include <malloc.h>
#include <vector>

RdbDatafile::RdbDatafile(string name, RdbVault *vault)
{
  /* Asign data to fields */
  this->name = name;
  this->path = vault->getPath() + "/" + vault->getName() + "/datastore/" + name;
  this->version = 1;
  
  /* By default things should only be part of one category */
  // this->category = 
  
  /* Then populate them */
  this->populate();
  
  /* Set cache mode to hotdrop so we can use the file immediately */
  this->cache(HOTDROP);
  
  this->write();
}

void RdbDatafile::populate()
{
  /* Create a list of two map pointers. One for our version revision, one for theirs */
  // this->data = new std::vector<map_t*>(2);
  // this->data[0] = hashmap_new();
  
  //this->data = new map_t*[2];
  // *this->data[0] = (map_t*) hashmap_new();
  
  /* Generate a sample value for our version */
  // hashmap_put((*this->data[0]), "Note", "I'm making a note here");
}
