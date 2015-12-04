#ifndef RDBDATAFILE_H
#define RDBDATAFILE_H

#include <string>
#include "RdbVault.h"

typedef enum CacheMode {
  /* Drop from cache after every action */
  HOTDROP, 
  
  /* Keep in cache for limited time (defined in config) */
  TIMEOUT, 
  
  /* Keep in cache until daemon shuts down */
  ENDLESS
};

class RdbDatafile
{
public:
  RdbDatafile(string name, RdbVault vault);
  
  /**
   * 
   */
  ree_err_t insertData(string data);
  
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
  ree_err_t delete();
  
private:
  
  /* Some global metadata */
  string name;
  string path;
  int version;
  
  bool locked;
  
  /* Actual data*/
  map_t data;
  
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
