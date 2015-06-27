/*
 * Vault.cpp
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "../public/vault.h"

Vault::Vault() {
	// TODO Auto-generated constructor stub

}

Vault::~Vault() {
	// TODO Auto-generated destructor stub
}

/*
 * Creates a vault on the filesystem and register it in the central config.
 * Creating a vault will return a token to the caller unless Reedb operates
 * in no-token mode. Then it will return a NULL reference.
 *
 * 	param [string] name
 *					The name of the vault will  have on the filesystem.
 *
 * 	param [string] path
 * 					The path where the vault will be created.
 *
 * 	param [string] passphrase
 * 					The passphrase that will be used to encrypt the initial cipher key
 * 					and files will be encrypted by.
 */
string Vault::createVault(string name, string path, string passphrase) {
	return NULL;
}
