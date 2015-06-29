#include "../lib/public/core.h"
#include "../lib/public/reec.h"
#include "../lib/public/reedb.h"

/**
 * Some main function to test shit.
 */
int main(int argc, char *argv[]) {

	options my_options { OS_Type::Linux, 16 };
	my_options.verbose = true;

	Reedb *rdb = new Reedb();
	rdb->core()->init(my_options);

	// Cleanup here
	delete rdb;
	return 0;
}
