/* libreedb - errors.h
 *
 * Errors are important for understanding why certain issues might
 * appear in your application. Almost every function in reedb returns
 * the custom rdb_err_t enum which means that even errors from
 * deeper in the database stack can propagate to the surface for
 * you to handle
 *
 * Each error contains a message describing the issue as in
 * addition to a well-defined error code.
 *
 * (c) 2015-2016                Lonely Robot.
 * Authors:                     Katharina 'spacekookie' Sabel
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
 * Additionally for error handling the function rdb_error_getcode(rdb_err_t *e);
 * can be used to map all possible errors to their codes as they are
 * determined at compile-time of the application.
 *
 * Any function you call should have a check for the error return.
 *
 * Good luck :)
 */

/* Make sure we're not included multiple times */
#ifndef RDB_ERRORS_H
#define RDB_ERRORS_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

/** Define some generic error codes first that we can propagate **/
typedef enum rdb_err_t {

    /* General purpose error codes */
    FAILURE = -1,                   // To be used when cause of error not known.
    SUCCESS = 0,                    // When something went according to plan.
    HUGE_SUCCESS = 0,               // Little Portal easter egg :)

    INVALID_PARAMS,
    MALLOC_FAILED,                  // A memory allocation couldn't be completed!
    INVALID_PAYLOAD,                // A common data return problem
    INVALID_BUFFER_SIZE,

} rdb_err_t;

unsigned int rdb_error_getcode(rdb_err_t *e);

const char *rdb_error_getmsg(rdb_err_t *e);

#ifdef __cplusplus
}
#endif
#endif /* RDB_ERRORS_H */