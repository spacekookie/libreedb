/*
 * Vault.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#ifndef VAULT_H_
#define VAULT_H_

#include <uuid.h>;

#include <string>
using namespace std;

class Vault {
public:
	Vault();
	virtual ~Vault();

	void scopeVault(string, string);
	void unscopeVault(uuid_t);

	void createVault(string, string, string);

	void insert();
	void remove();

	void getHeaders(uuid_t, string);
	void getFile(uuid_t, string, bool);

	void closeVault();
};

#endif /* VAULT_H_ */
