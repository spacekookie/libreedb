#include "TestClass.h"
#include <stdio.h>

#include "../includes/Reedb.h"

/**
 * Some main function to test shit.
 */
int main(int a, char**params) {
	Reedb::core()->init();

	return Reedb::core()->terminate();
}
