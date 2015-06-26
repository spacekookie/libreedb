/*
 * Reedb.cpp
 *
 *  Created on: 26 Jun 2015
 *      Author: spacekookie
 */

#include "../includes/Reedb.h"

Reedb::Reedb() {
	this->_core = new Core();
}

Reedb::~Reedb() {

}

Core *Reedb::core() {
	return getInstance()._core;
}

Config *Reedb::config() {
	return getInstance()._config;
}

Vault *Reedb::vault() {
	return getInstance()._vault;
}

Daemon *Reedb::daemon() {
	return getInstance()._daemon;
}
