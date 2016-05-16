#ifndef RDBDATAFILE_H
#define RDBDATAFILE_H

#include <string>

#include <vector>
#include <map>

#include "crypto/rcry_token.h"
#include "crypto/rcry_engine.h"

#include "protos/rdb_data.pb.h"

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
} datafile_h;

class datafile {
public:

    /**
     * Default constructor that creates a datfile. It goes through
     * the entire file creation process and optionally also creates
     * new keys for zones it should be long to.
     *
     * @param name Name of the file (in cleartext)
     * @param *parent Struct that contain parent path + permissions
     */
    datafile(string name, string parent);

    /**
     * Use this constructor to initiate a file that has previously
     * existed.
     *
     * @param name Name of the file (in cleartext)
     * @param oldFile Contents on the old file as binary buffer
     */
    datafile(string name, reedb_proto::rdb_data *old_file);

    /**
     * Before an insert this function should be called to add a new revision to
     * the proto data tree and return it's pointer.
     */
    reedb_proto::rdb_data::revision *incr_revision();

    /**
     * Inserts a key-value pair into a revision. For a multi-key-val insert this
     * function can be called in a loop.
     */
    void insertData(reedb_proto::rdb_data::revision *rev, string key, string val);

    /**
     *
     */
    // void readFile(map *(*data));

    /**
     * @param mode: The mode that should be used for caching. See @"CacheMode"
     */
    void cache(const cache_mode mode);


    /**
     * Fills the protocol buffer, pipes the bytestream through the
     * crypto engine and then dumps that to disk as base64 encoded
     */
    void write(rcry_engine *engine, rcry_token *token);

    map<string, string> *read(rcry_engine *engine, rcry_token *token);

    void populate_header(datafile_h *dh);

    /**
     * Closes the file and uncaches it.
     */
    void close();

    /** Serialise a map of stuff into a string */
    string serialise(reedb_proto::rdb_data *proto);

    /** Deserialise a string back into a map of stuff */
    reedb_proto::rdb_data *deserialise(string data);

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
    //std::vector<std::map<string, void *> *> *data;
    reedb_proto::rdb_data *data;

    /* @return If file is locked */
    bool isLocked();

    /**
     * Populate a new file with basic data so there are no nullpointers
     * that can get thrown.
     */
    void populate();
};

#endif // RDBDATAFILE_H
