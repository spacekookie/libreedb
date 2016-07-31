/* libreedb - config.h
 *
 * A header only file that contains definitions for config values, default
 * ranges as well as indexes that can easily be substituted into different
 * parts of the database.
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

#ifndef REEDB_CONFIG_H
#define REEDB_CONFIG_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif


#define CFG_SCOPE               "scoped"
#define CFG_CREATION_DATE       "creation_date"
#define CFG_MODIFY_DATE         "last_modified"
#define CFG_SCL_TYPE            "scale_type"
#define CFG_SCL_FACTOR          "scale_factor"

#define CFG_SCL_FIXED           "fixed"
#define CFG_SCL_DYNAMIC         "dynamic"
#define CFG_SCL_INF             "infinite"

#define CFG_SCOPE_NAME          "name"
#define CFG_SCOPE_UUID          "uuid"
#define CFG_SCOPE_PATH          "path"
#define CFG_SCOPE_SIZE          "size"

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
}
#endif


#endif //REEDB_CONFIG_H
