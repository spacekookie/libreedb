/*
 * Core.cpp
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "../public/core.h"

#include <cstdio>
#include <iostream>
#include <string>

#include "utils/utilities.h"

using namespace std;

Core::Core() {
}

Core::~Core() {
}

void Core::init(options options) {
	string uuid = Utilities::generateUuid();
	cout << uuid << endl;
}

int Core::terminate() {

	printf("Welcome!\n");
	return 0;
}
