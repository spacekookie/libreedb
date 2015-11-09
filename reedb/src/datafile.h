/*
 * (c) 2015 Lonely Robot.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 *
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 *
 * Author: Katharina 'spacekookie' Sabel
 *
 */

#ifndef SRC_DATAFILE_H_
#define SRC_DATAFILE_H_

/* System requirements */
#include <stdbool.h>
#include <stdlib.h>

#include "reedb/utils/hashmap.h"
#include "reedb/defs.h"

/** Defines the type of data that should be */
typedef enum gen_data_t {
  string, integer, boolean
} gen_data_t;

/*
 * This struct can hold generic data. It carries a type
 * and size to determine as what data type and field size
 * it should be read.
*/
typedef struct gen_data {
  gen_data_t  type;
  size_t      size; // > 1 means it's an array.

  union v {
    int       *iptr;
    char      *cptr;
    bool      *bptr;
  } v;
} gen_data;

/** Defines the file header which is passed out to users */
typedef struct datafile_h {
  char      *name;
  char      *category;
  map_t     *tags;
} datafile_h;

/** Definition of the actual datafile struct. Not exposed to user */
typedef struct datafile {
  /* Some metadata (again) */
  char          *name;          // String name. Is hashed for the filesystem name
  datafile_h    *header;        // Pointer to header struct. Used for indexing
  size_t        bsize;          // Size of the body map (revision numbers)
  size_t        rsize;          // The amount of revisions that CAN be stored

  /* Actual datastorage (contains 25% more crypto than the competition) */
  map_t         **body;         // Nested body maps for revisions and data
  map_t         *locks;         // Stores locks on specific versions.
} datafile;

/** Creates a new file with a name */
ree_err_t create_file(datafile **file, char *name);

/** Needs to be called before a transaction to prevent race conditions */
ree_err_t lock_file(datafile *file);

/** Needs to be called again after a transaction */
ree_err_t unlock_file(datafile *file);

ree_err_t insert_data(datafile *file, char *field, gen_data *data);

ree_err_t delete_data(datafile *file, char *field);

/** Needs to be called to unlock a revision again and finalize the changes */
ree_err_t finalize_version(datafile *file);

ree_err_t update_header(datafile *file, ...);

ree_err_t get_header(datafile *file, datafile_h **headreq);

ree_err_t read_file(datafile *file, bool versioning);

/** Needs to be called to sync in-memory file with disk-file. Will dump
 *  encrypted data to disk. Blocking operation until file lock is not present */
ree_err_t sync(datafile *file, char mode);

ree_err_t close(datafile *file);

ree_err_t remove_file(datafile *file);

#endif /* SRC_DATAFILE_H_ */
