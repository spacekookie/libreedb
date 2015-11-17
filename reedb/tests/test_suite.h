/* reedb/tests - test_suite
 *
 * This file kicks off the automated unittest suite of the reedb core. It tests
 * all primary functions like (but not limited to) encryption, decryption, key
 * generation, validity of secure memory, resource leaks, file handling, data
 * integrity, versioning and version integrity checking.
 *
 * TODO: Hook this file into the cmake build process so it runs this test suite
 * every time it builds. CI and stuff.
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
 */

#ifndef REEDB_TEST_SUITE_H
#define REEDB_TEST_SUITE_H

#include "reedb/defs.h"

/**
 * Use this function to hook your own tests into the reedb build test suite. It will
 * store the function pointer in a collection and run stored test functions sequentially.
 *
 * Just make sure your source file is included in the cmake build. And that this header
 * is included in your source file. For more information please consult the wiki.
 *
 * === This function takes three arguments ===
 * void*    A function pointer
 * int      Number of arguments your function takes
 * char**   An array of arguments you want to pass into your function (like main)
 */
ree_err_t rdb_test_hook(void *funct, int argn, char **argv);

#endif //REEDB_TEST_SUITE_H
