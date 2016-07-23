/* libreedb - reedb.c
 *
 * This file defines all the public core functions for libreedb. From
 * it you will be able to create management instances as well as
 * standalone vaults with the respective runtime and configuration flags.
 * For more information, please consult the official documentation or
 * the repository wiki.
 *
 * (c) 2016 					Lonely Robot.
 * Authors:						Katharina 'spacekookie' Sabel
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * -------------------------------------------
 *
 * A (hopefully) logical API :)
 */

/******** Include statements first ********/
#include <reedb/reedb.h>

////////////////////////////////////////////////////
/////////////////                  ////////////////
//////////////// MANAGEMENT CLASS ////////////////
///////////////                  ////////////////
////////////////////////////////////////////////



/** Simple constructor */
rdb_core::rdb_core() {

    /** Create a handle for active vaults */
    this->active_vaults = new map<rdb_uuid*, rdb_vault*>();

    /** Create buffer for vault scope */
    this->vault_scope = new list<vault_meta*>();

}

/** Cleans up all resources */
rdb_core::~rdb_core() {

}

/** Creates a new vault object to play with */
rdb_vault* rdb_core::create_vault(string name, string path) {
    rdb_vault *vault = new rdb_vault(name, path);
    return vault;
}
