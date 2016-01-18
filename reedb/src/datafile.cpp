
/* Internal Reedb requirements */
#include "datafile.h"

/* Some system requirements for data storage and file access*/
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

datafile::datafile(string name, file_path *parent)
{
  /* Asign data to fields */
  this->name = name;
  this->path = *parent->path + "/" + name;
  this->version = 1;

  std::cout << "Just created a new datafile" << endl;
  
  /* Then populate them */
  this->populate();
  
  /* Set cache mode to hotdrop so we can use the file immediately */
  this->cache(HOTDROP);
  this->write();
  
  std::cout << "File dump complete. Ready to cache it from disk..." << endl;
}

void datafile::populate()
{  
  this->data = new vector<map<string, void*>*>;
}

void datafile::write()
{

}

void datafile::cache(const cache_mode mode)
{
  
}
