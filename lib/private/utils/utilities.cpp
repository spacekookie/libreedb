/*
 * Utilities.cpp
 *
 *  Created on: 27 Jun 2015
 *      Author: spacekookie
 */

#include "utilities.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

using namespace std;

Utilities::Utilities() {
}

Utilities::~Utilities() {
}

const short UUID_LENGTH = 32;
const short UUID_SECTIONS = 4;

string Utilities::generateUuid() {
	const char *sequence = "0123abcdefg456hijklmnop789qrstuvwxyz";
	const int ALPH_SIZE = strlen(sequence);

	char *uuid = new char[UUID_LENGTH];

	/** Setup the random seed */

	srand(time(NULL));
	for (int n = 0; n < UUID_LENGTH; n++) {
		int picker = rand() % ALPH_SIZE;

		uuid[n] = sequence[picker];
	}

	/** Now split up the UUID into sections again*/
	int splitter = UUID_LENGTH / UUID_SECTIONS;

	for (int c = 1; c < UUID_SECTIONS; c++) {
		uuid[splitter * c] = '-';
	}

	return uuid;
}
