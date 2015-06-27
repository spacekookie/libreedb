/*
 * Core.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "../private/constants.h"

#ifndef CORE_H_
#define CORE_H_

class Core {

private:
	bool active;
	char *path;
	char *user;
	OS_Type os;

public:
	Core();
	virtual ~Core();

	/** Init the*/
	void init();

	/** Terminate the currently running instance with a user reason defined in the constants.h */
	int terminate();
};

#endif /* CORE_H_ */
