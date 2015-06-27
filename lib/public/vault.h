/*
 * Vault.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#ifndef VAULT_H_
#define VAULT_H_

#include <uuid.h>
#include <string>

using namespace std;

class Vault {
public:
	Vault();
	virtual ~Vault();

	void scopeVault(string, string);
	void unscopeVault(uuid_t);

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
	string createVault(string, string, string);

	void insert();
	void remove();

	void getHeaders(uuid_t, string);
	void getFile(uuid_t, string, bool);

	void closeVault();
};

#endif /* VAULT_H_ */
