/*
 * core.h
 *
 *  Created on: 5 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_CORE_H_
#define SRC_CORE_H_

/*
 * (C) Copyright 2014-2015 Lonely Robot.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

/*
 * This header file contains handles to interact with the Reedb core module.
 * The core module is responsible for starting and terminating running Reedb
 * instances as well as checking that all other modules are loaded and
 * running correctly.
 *
 * @author: Katharina 'spacekookie' Sabel <sabel.katharina@gmail.com>
 */

#include <stdbool.h>
#include "utils.h"

typedef enum ree_os {
	LINUX = 0xE1, /* Linux systems */
	OSX = 0xE2, /* Mac OS X */
	WINDOWS = 0xE3, /* Windows */
	ANDROID = 0xE4, /* Mobile: Android */
	IOS = 0xE5, /* Mobile: iOS */
	BSD = 0xE6, /* BSD systems */
} ree_os;

/** Main Reedb container that gets passed around modules */
typedef struct reedb_c {
	bool active;
	unsigned int passlength; //
	bool verbose; //
	bool daemon; //
	char *path;
	ree_os os;
} reedb_c;

/** Sets the minimum passphrase length for Reedb */
ree_err_t rdb_set_passlength(unsigned int length);

/** Provide a custom user function to execute concurrent to the Reedb threads */
ree_err_t rdb_set_usrcode(void *funct);

/** Make Reedb run in daemon mode. This detaches all logging to file and makes the runtime more autonomous. */
ree_err_t rdb_set_daemon(bool daemon);

/** Enable verbose debugging either to log or to STDOUT/ STDERR */
ree_err_t rdb_set_verbose(bool verbose);

/** Set the operational path of Reedb. This is automatically set by default */
ree_err_t rdb_set_path(char *path);

/** The OS should be parsed automatically. You can override it if you think this is a mistake */
ree_err_t rdb_set_os(ree_os os);

/** Mark that Reedb should override existing zombie instances at the risk of causing serious damage! */
ree_err_t rdb_set_override(bool override);

/** Actually init the core module after setting up all runtime parameters */
ree_err_t reedb_init(reedb_c *(*container));

/** Terminate the current Reedb instance and */
ree_err_t reedb_terminate(char *reason);

bool reedb_isActive();

#endif /* SRC_CORE_H_ */
