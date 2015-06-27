/*
 * Utilities.cpp
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "../../includes/utils/Utilities.h"

#include <string>
#include <stdlib.h>
#include <cmath>
#include <time.h>
using namespace std;

Utilities::Utilities() {
}

Utilities::~Utilities() {
}

short UUID_LENGTH = 16;
short UUID_SECTIONS = 4;


string Utilities::generateUuid() {
	const char alphanum[] = "0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

	char *uuid = new char[UUID_LENGTH];

	for (int n = 0; n < UUID_LENGTH; n++) {
		uuid[n] = alphanum[rand() % (sizeof(alphanum)) - 1];
	}


	/** Now split up the UUID into sections again*/
	int splitter = UUID_LENGTH / UUID_SECTIONS;

	for(int c = 1; c < UUID_SECTIONS; c++) {
		uuid[splitter * c] = '-';
	}

	return uuid;
}
