/*
 * Utilities.h
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#ifndef UTILS_UTILITIES_H_
#define UTILS_UTILITIES_H_

#include <string>

using namespace std;

class Utilities {
public:
	Utilities();
	virtual ~Utilities();

	static string generateUuid();
};

#endif /* UTILS_UTILITIES_H_ */
