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
 *
 * If you want to add a unit test please add an apropriate .c and .h file into the
 * tests folder or subsequent subdirectories. Add the source file to the cmake build
 * file. And then use the #{rdb_test_hook} function to bootstrap your own unit tests
 * into the main suite.
 */

#include "reedb/defs.h"

typedef struct rdb_tf
{
    rdb_tf        *next;
    ree_err_t     result;
    void          *function;
    char          **argv;
    int           argn;

} rdb_tf;

static rdb_tf *test_list = NULL;

ree_err_t rdb_test_hook(void *funct, int argn, char **argv)
{
    if(test_list == NULL)
    {
        test_list = malloc(sizeof(rdb_tf));
        if(test_list == NULL) return MALLOC_FAILED;

        test_list->function = funct;
        test_list->argn = argn;
        test_list->argv = argv;
        test_list->result = NULL;
        test_list->next = NULL;
    } else {

        /* Goes through the entire test list and stops iterating at the last item */
        rdb_tf *check = test_list->next;
        while(true)
        {
            if(check->next == NULL)
            {
                check->next = malloc(sizeof(rdb_tf));
                if(check->next == NULL) return MALLOC_FAILED;

                check->next->function = funct;
                check->next->argn = argn;
                check->next->argv = argv;
                check->next->result = NULL;
                check->next->next = NULL;
                break;
            }
            check = check->next;
        }
    }
    return SUCCESS;
}

int main(int argn, char **argv) {

    return 0;
}