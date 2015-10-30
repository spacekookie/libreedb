/* reedb - utils/files.h
 *
 * Header to a bunch of file and path utilities making concatinations
 * and getting full paths easier.
 *
 * (c) 2015 					Lonely Robot.
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

#include <stdarg.h>

#ifndef SRC_FILES_UTILS_H_
#define SRC_FILES_UTILS_H_

/*  */
char *rdb_concat_path_simple(char *path, char *directory);

/* Concats something to a filename */
char *rdb_concat_fname_simple(char *path, char *directory);

/* Concat a bunch of strings to a path. */
char *rdb_concat_path(int count, ...);

/* Expand a path to an absolute path */
char *rdb_absolute_path(char *path);

#endif