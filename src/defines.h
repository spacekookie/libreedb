/* libreedb - defines.h
 *
 * This file contains a lot of structure definitions required for vaults
 * to function properly as well as constant values that shouldn't be exposed
 * to the outside.
 *
 * TODO: Fill in this description
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
 */

#ifndef REEDB_DEFINES_H
#define REEDB_DEFINES_H

#include <time.h>
#include "utils/uuid.h"

#define MINIMUM_BUFFER_SIZE         4
#define DEFAULT_BUFFER_SIZE         8
#define PASSWD_HASH_FUNCT           GCRY_MD_TIGER2
#define PASSWD_HASH_LEN             gcry_md_get_algo_dlen(PASSWD_HASH_FUNCT)

#define USER_HASH_FUNCT             GCRY_MD_SHA512
#define USER_HASH_LEN               gcry_md_get_algo_dlen(USER_HASH_FUNCT)

#define SALT_SEPERATOR              ":::"

typedef struct vault_user {
    rdb_uuid    *id;
    char        *name, *secname, *passwd;
    time_t      last_login, created;
};

typedef struct vault_zone {
    rdb_uuid            id;
    char                *name;
    struct vault_user   **access;
};

#endif //REEDB_VAULT_DEFS_H
