#include "../lib/public/core.h"
#include "../lib/public/reedb.h"

/**
 * Some main function to test shit.
 */
int main(int a, char**params) {
	Reedb::core()->init();

	Reedb::vault()->createVault(NULL, NULL, NULL);

	Reedb::core()->terminate();

	return 0;
}
