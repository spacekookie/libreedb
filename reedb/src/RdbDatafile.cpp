
/* Internal Reedb requirements */
#include "RdbDatafile.h"
#include "reedb/utils/hashmap.h"

/* Some system requirements for data storage and file access*/
#include <list>
#include <fstream>
#include <iostream>
#include <malloc.h>

using namespace std;

RdbDatafile::RdbDatafile(string name, FilePath *parent)
{
  /* Asign data to fields */
  this->name = name;
  this->path = *parent->path + "/" + name;
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
  list<int> first;
  first.push_back(5);
  first.push_front(2);
  first.push_back(9);
  
  for(int x : first) {
    cout << x << endl;
  }
  
  cout << "First number: " << first.front() << endl;

  
  
  // Try to insert something in the sucker
  //hashmap_put(&hashmap, "Reedb", "I am some awesome data");
  
  //data.push_back(&revision);
  
  //hashmap_put(&revision, "Note", "I'm making a note here, huge success!");
  
  
//   list<map_t*> data;
//   
//   map_t revision = hashmap_new();
//   data.push_back(&revision);
//   revision = NULL; // Remove the pointer again. It's wasteful

    
    
  // this->data = new std::vector<map_t*>(2);
  // this->data[0] = hashmap_new();
  
  //this->data = new map_t*[2];
  // *this->data[0] = (map_t*) hashmap_new();
  
  /* Generate a sample value for our version */
  // hashmap_put((*this->data[0]), "Note", "I'm making a note here");
}

ree_err_t RdbDatafile::cache(const CacheMode mode)
{

}

void RdbDatafile::write()
{

}
