#ifndef RDBDATAFILE_H
#define RDBDATAFILE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

typedef enum cache_mode {
  /* Drop from cache after every action */
  HOTDROP, 
  
  /* Keep in cache for limited time (defined in config) */
  TIMEOUT, 
  
  /* Keep in cache until daemon shuts down */
  ENDLESS
} cache_mode;

typedef struct file_path {
  string *path;
  int opMode;
} file_path;

/** 
 * Struct that holds a file header. Is part of a datafile but 
 * is used here to allow iteration and caching of the head for
 * quick access via RQL
 */
typedef struct datafile_h {
  string *name;
  string *tags;
  string category;
  
  unsigned int size;
  unsigned int version;
  bool locked;
} datafile_h ;

class datafile
{
public:
  
  /** 
   * Default constructor that creates a datfile. It goes through
   * the entire file creation process and optionally also creates
   * new keys for zones it should be long to.
   * 
   * @param name Name of the file (in cleartext)
   * @param *parent Struct that contain parent path + permissions
   */
  datafile(string name, file_path *parent);
  
  /** 
   * Use this constructor to initiate a file that has previously 
   * existed.
   * 
   * @param name Name of the file (in cleartext)
   * @param oldFile Contents on the old file as binary buffer
   */
  datafile(string name, string oldFile);
  
  /**
   * 
   */
  void insertData(string data);
  
  /**
   * 
   */
  // void readFile(map *(*data));
  
  /**
   * @param mode: The mode that should be used for caching. See @"CacheMode"
   */
  void cache(const cache_mode mode);
  
  /**
   * Closes the file and uncaches it.
   */
  void close();
  
  /**
   * Deletes this datafile, scrubs it from disk and removes the encryption
   * keys from the keystore.
   */
  
private:
  
  /* Some global metadata */
  string name;
  string path;
  int version;
  bool locked;
  
  /* Database metadata for queries */
  string *category;
  string *tags;
  
  /* Vector of data maps. Each version creates a new map */
  std::vector<std::map<string, void*>*> *data;
  
  /* @return If file is locked */
  bool isLocked();
  
  /** 
   * Populate a new file with basic data so there are no nullpointers
   * that can get thrown.
   */
  void populate();
  
  /** 
   * Fills the protocol buffer, pipes the bytestream through the 
   * crypto engine and then dumps that to disk as base64 encoded
   */
  void write();
  
};

#endif // RDBDATAFILE_H
