/*
 * Reedb.cpp
 *
 *  Created on: 26 Jun 2015
 *      Author: spacekookie
 */

#include "../public/reedb.h"

Reedb::Reedb() {
	this->_core = new Core();
	this->_config = new Config();
	this->_vault = new Vault();
	this->_daemon = new Daemon();
}

Reedb::~Reedb() {

}

Core *Reedb::core() {
	Reedb::getInstance();
	return getInstance()._core;
}

Config *Reedb::config() {
	Reedb::getInstance();
	return getInstance()._config;
}

Vault *Reedb::vault() {
	Reedb::getInstance();
	return getInstance()._vault;
}

Daemon *Reedb::daemon() {
	Reedb::getInstance();
	return getInstance()._daemon;
}
