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

#include "cppwrapper.h"
#include "datafile.h"

extern "C" {
#include "reedb/defs.h"
}

#include <iostream>

ree_err_t cpp_datafile_testing(){
  FilePath *fp = new FilePath;
  *fp->path = "/home/spacekookie/";
  
  std::cout << "I am some awesome C++ code!" << std::endl;
  
  datafile *df = new datafile("default", fp);
  delete(df);
  return SUCCESS;
}