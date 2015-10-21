/* reedbd-http - main.c
 * 
 * This file is the entry point to the Reedb HTTP(s) extention.
 * It is statically compiled against Reedb and adds functions for
 * web applications and usability without using the C bound API.
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
 * Initialisation of reedbd-http is done automatically (mostly).
 */

#include "reedb/utils/hashmap.h"
#include "reedb/vault.h"
#include "reedb/core.h"

#include <string.h>
#include <stdio.h>

int iteration(char *array)
{
	printf("YES! ");
	printf("Also: %s\n", array);
	return 0;
}

int main(int argc, char *args)
{
// Use debugging
#define RDB_DEBUG true

	map_t *testmap = hashmap_new();

	char *one = malloc(sizeof(char) * 8);
	one = "AAAAAAA";

	char *two = malloc(sizeof(char) * 8);
	two = "BBBBBBB";

	char *three = malloc(sizeof(char) * 8);
	three = "CCCCCCC";

	char *four = malloc(sizeof(char) * 8);
	four = "DDDDDDD";

	hashmap_put(testmap, "one", one);
	hashmap_put(testmap, "two", two);
	hashmap_put(testmap, "three", three);
	hashmap_put(testmap, "four", four);

	hashmap_iterate(testmap, iteration, one);

	/* Initialise Reedb container instance */
	// reedb_c *rdb;
	// ree_err_t error;

	// rdb_set_passlength(12);
	// rdb_set_os(LINUX);

	// /* Call the init */
	// error = reedb_init(&rdb);
	// printf("Init returning with code: %d\n", error);

	// /* Init vault module */
	// rdb_vault_init(&rdb);

	// /* Handles for vault ID stuff */
	// ree_token 		*token;
	// ree_uuid			*uuid;
	// error = rdb_vault_create(&token, &uuid, "default", "/home/spacekookie", "super_password");
	// printf("Create vault returned with code %d\n", error);

	// /* Terminate our instance. Frees up all sensitive information from memory */
	// error = reedb_terminate(&rdb, "Program ran out of lines of code");
	
	// printf("Returning with code: %d\n", error);
	return 0;
}