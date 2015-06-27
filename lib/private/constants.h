/*
 * Constants.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

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
	bool daemon = true;
	bool verbose = false;
	bool token = true;

};

#endif /* CONSTANTS_H_ */
