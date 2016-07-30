/* libreedb - platform.h
 *
 * Utility file that contains helper functions for platform specific tasks.
 * In most cases it wraps either around compile-time defined fields
 * or posix API.
 * It is considered useful enough to expose functionality to the user
 * to use in her applications.
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

#ifndef REEDB_PLATFORM_H
#define REEDB_PLATFORM_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif


#define RDB_PLATFORM_LINUX      0x0
#define RDB_PLATFORM_FREEBSD    0x1
#define RDB_PLATFORM_UNIX       0x2
#define RDB_PLATFORM_OSX        0x3
#define RDB_PLATFORM_WIN        0x4

/** Get the current OS type which is supported */
unsigned int rdb_platform_getos();

/** Get the local machine name */
const char *rdb_platform_getmachine();

/** Get the current username */
const char *rdb_platform_getuser();

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
}
#endif
#endif //REEDB_PLATFORM_H
