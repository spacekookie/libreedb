#ifndef RDBDATAFILE_H
#define RDBDATAFILE_H

#include <string>
#include "RdbVault.h"

extern "C" {
#include "reedb/utils/hashmap.h"
}

typedef enum CacheMode {
  /* Drop from cache after every action */
  HOTDROP, 
  
  /* Keep in cache for limited time (defined in config) */
  TIMEOUT, 
  
  /* Keep in cache until daemon shuts down */
  ENDLESS
} CacheMode;

/*
 * This struct can hold generic data. It carries a type
 * and size to determine as what data type and field size
 * it should be read.
*/


class RdbDatafile
{
public:
  RdbDatafile(string name, RdbVault *vault);
  
  /** Use this constructor to initiate a file that has previously existed */
  RdbDatafile(string name, string oldFile);
  
  /**
   * 
   */
  ree_err_t insertData(string data);
  
  /**
   * 
   */
  ree_err_t readFile(map_t *(*data));
  
  /**
   * @param mode: The mode that should be used for caching. See @"CacheMode"
   */
  ree_err_t cache(const CacheMode mode);
  
  /**
   * Closes the file and uncaches it.
   */
  ree_err_t close();
  
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
  
  /* Array of data maps. Each version creates a new map */
  map_t *data[];
  
  /* @return If file is locked */
  bool isLocked();
  
  /** 
   * Populate a new file with basic data so there are no nullpointers
   * that can get thrown.
   */
  void populate();
  
  /**
   * Writes the data from 
   */
  void write();
  
};

#endif // RDBDATAFILE_H
