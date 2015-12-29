
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
  
  /* By default things should only be part of one category */
  // this->category = 
  
  std::cout << "Just created a new datafile" << endl;
  
  /* Then populate them */
  this->populate();
  
  std::cout << "Preparing to set cache mode" << endl;
  
  /* Set cache mode to hotdrop so we can use the file immediately */
  this->cache(HOTDROP);
  
  std::cout << "Cache mode set...done!" << endl;
  
  this->write();
  
  std::cout << "File dump complete. Ready to cache it from disk..." << endl;
}

void datafile::populate()
{  
//   std::cout << "Populating new file with sample data..." << endl;
//   
//   /* Create a list of hashmap pointers */
//   this->data = new vector<map<string, void*>*>;
  
  std::cout << "Debug 1" << endl;

//   map<std::string, void*> sample; // = new map<std::string, void*>;

  std::cout << "Debug 2" << endl;
  
  string *sampleNote = new string("This is an example note. Feel free to de");
  
  // string *note = new string("This is an example note. Feel free to delete it!");

  //sample["Note"] = note;

  std::cout << "Debug 3...for real!" << endl; 
  
  delete(sampleNote);
}
