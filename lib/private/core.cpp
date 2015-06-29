/*
 * Core.cpp
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "../public/core.h"
#include "../public/reec.h"
#include "constants.h"

#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string>

#include "utils/utilities.h"

using namespace std;

Core::Core() {
}

Core::~Core() {
}

void Core::init(options options) {
	printf("Hello!\n");
	if (options.path == DEFAULT_PATH) {
		switch (options.os) {
		case OS_Type::Linux:
			this->path = &DEFAULT_LINUX;
			break;
		case OS_Type::OSX:
			this->path = &DEFAULT_OSX;
			break;
		case OS_Type::Win:
			this->path = &DEFAULT_WIN;
			break;
		default:
			break;
		};
	}

}

int Core::terminate() {
	return 0;
}
