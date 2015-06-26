/*
 * Reedb.h
 *
 *  Created on: 26 Jun 2015
 *      Author: spacekookie
 */

#ifndef REEDB_H_
#define REEDB_H_

#include "Config.h"
#include "Core.h"
#include "Daemon.h"
#include "Vault.h"

class Reedb {

private:
	Reedb();
	Reedb(Reedb const& copy);
	Reedb& operator=(Reedb const& copy);

	Core *_core;
	Config *_config;
	Vault *_vault;
	Daemon *_daemon;

public:
	static Reedb& getInstance() {
		static Reedb instance;
		return instance;
	}

	virtual ~Reedb();
	/** Sub modules */

	static Core *core();
	static Config *config();
	static Vault *vault();
	static Daemon *daemon();
};

#endif /* REEDB_H_ */
