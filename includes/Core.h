/*
 * Core.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#ifndef CORE_H_
#define CORE_H_

class Core {

public:
	Core();
	virtual ~Core();

	/** Terminate the currently running instance with a user reason defined in the constants.h */
	void terminate();
};

#endif /* CORE_H_ */
