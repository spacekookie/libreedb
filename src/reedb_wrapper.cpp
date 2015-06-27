#include "../lib/private/constants.h"
#include "../lib/public/core.h"
#include "../lib/public/reedb.h"

/**
 * Some main function to test shit.
 */
int main(int argc, char *argv[]) {

	options my_options { OS_Type::Linux, 16 };
	//options *my_options = new options(OS_Type::Linux, 16);
	// Reedb::core()->init(&my_options);

	Reedb::core()->terminate();
	return 0;
}
