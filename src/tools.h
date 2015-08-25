/*
 * tools.h
 *
 *  Created on: 25 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include "defs.h"
#include "uuid.h"

ree_err_t search_active_vaults(ree_uuid *uuid);

ree_err_t get_actiuve_vault(ree_uuid *uuid);

#endif /* SRC_TOOLS_H_ */
