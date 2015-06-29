/*
 * Reedb.h
 *
 *  Created on: 26 Jun 2015
 *      Author: spacekookie
 */

#ifndef REEDB_H_
#define REEDB_H_

#include "vault.h"
#include "config.h"
#include "core.h"
#include "daemon.h"

class Reedb {

private:
	Core *_core;
	Config *_config;
	Vault *_vault;
	Daemon *_daemon;

public:
	/* Constructor & Destructor */
	virtual ~Reedb();
	Reedb();

	/** Sub modules */
	Core *core();
	Config *config();
	Vault *vault();
	Daemon *daemon();
};

#endif /* REEDB_H_ */
