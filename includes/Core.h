/*
 * Core.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "Constants.h"

#ifndef CORE_H_
#define CORE_H_

class Core {

private:
	char *path;
	char *user;
	OS os;

public:
	Core();
	virtual ~Core();

	/** Init the*/
	void init();

	/** Terminate the currently running instance with a user reason defined in the constants.h */
	void terminate();
};

#endif /* CORE_H_ */
