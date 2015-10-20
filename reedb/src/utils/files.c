/* reedb - utils/files.c
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
#include <string.h>

/* Concat a bunch of strings to a path. 
 * Returns a pointer to a heap string 
 */
char *rdb_concat_path(int count, ...)
{
	va_list valist;
	int length = 0;
	int i;

	/* Initialise the VA list for the number of arguments */
	va_start(valist, count);

	/* Get the total string length */
	for (i = 0; i < count; i++) 
	{ 
		char *string = va_arg(valist, int);

		/* Check that we are really only dealing with strings here */
		length += strlen(va_arg(valist, int));
	}	

	char *path = malloc(sizeof(char) * length);
	
	/* Get the total string length */
	for (i = 0; i < count; i++) 
	{ 
		strcpy(path, va_arg(valist, int));
	}	
	return path;	
}

/* Expand a path to an absolute path */
char *rdb_absolute_path(char *path)
{

}