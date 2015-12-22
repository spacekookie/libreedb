
/* Internal Reedb requirements */
#include "RdbDatafile.h"
#include "reedb/utils/hashmap.h"

/* Some system requirements for data storage and file access*/
#include <iostream>
#include <vector>

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
  
  /* Create a list of hashmap pointers */
  vector<map_t*> *revisions = new vector<map_t*>;
  
  /* Create a hashmap for some sample data */
  map_t samples = hashmap_new();
  char *data = "This is an example note. Feel free to delete it!";
  hashmap_put(&samples, "Note", data);

  /* Then creating two hashmap pointers for data! \o/*/
  // revisions->push_back();
  // revisions->push_back(hashmap_new());
  
  /* Then put some sample data into the first map*/
  // hashmap_put(*revisions[0], "Label", "This is an example entry");
  

  /* Create a list of two map pointers. One for our version revision, one for theirs */
  //   list<int> first;
  //   first.push_back(5);
  //   first.push_front(2);
  //   first.push_back(9);
  //   
  //   for(int x : first) {
  //     std.cout << x << endl;
  //   }
  //   
  //   cout << "First number: " << first.front() << endl;

  
  
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
