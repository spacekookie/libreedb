/*
 * Core.cpp
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "../includes/Core.h"
#include "../includes/utils/Utilities.h"

#include <stdio.h>
#include <string>


using namespace std;

Core::Core() {
}

Core::~Core() {
}

void Core::init() {
	string uuid = Utilities::generateUuid();
	printf("%s\n", uuid);
}

int Core::terminate() {

	printf("Welcome!\n");
	return 0;
}
