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
	return this->_core;
}

Config *Reedb::config() {
	return this->_config;
}

Vault *Reedb::vault() {
	return this->_vault;
}

Daemon *Reedb::daemon() {
	return this->_daemon;
}
