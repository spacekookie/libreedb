#include "TestClass.h"
#include <stdio.h>

#include "../includes/Reedb.h"

/**
 * Some main function to test shit.
 */
int main(int a, char**params) {
	Reedb::getInstance();

	printf("Init Reedb done! \n");

	Reedb::core()->terminate();

	fflush(stdout);

	printf("Here\n");

	return 0;
}
