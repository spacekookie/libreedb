/*
 * reec.h
 *
 *  Created on: 28 Jun 2015
 *      Author: spacekookie
 */

#ifndef PUBLIC_REEC_H_
#define PUBLIC_REEC_H_

#include "../private/constants.h"
#include <string>

enum class OS_Type {
	Linux, OSX, Win
};

struct options {
	options(OS_Type os, unsigned pw_length) :
			os(os), pw_length(pw_length) {
	}

public:
	OS_Type os;
	unsigned pw_length;
	std::string path = DEFAULT_PATH;
	bool daemon = true;
	bool verbose = false;
	bool token = true;

};

#endif /* PUBLIC_REEC_H_ */
