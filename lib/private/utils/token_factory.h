/*
 * TokenFactory.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#ifndef UTILS_TOKENFACTORY_H_
#define UTILS_TOKENFACTORY_H_

#include <string>

using namespace std;

class TokenFactory {
public:
	TokenFactory();
	virtual ~TokenFactory();

	string generateToken();

};

#endif /* UTILS_TOKENFACTORY_H_ */
