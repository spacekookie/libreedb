#include "TestClass.h"
#include <stdio.h>

#include "../includes/Reedb.h"

#include <map>
#include <iostream>
#include <cassert>

int main(int argc, char **argv) {
	std::map<short, short> m;
	//m["hello"] = 23;

	//if (m.find("world") != m.end()) std::cout << "map contains key world!\n";

	//std::cout << m["hello"] << '\n';
	//std::map<short, int>::iterator i = m.find("hello");
	//assert(i != m.end());
	//std::cout << "Key: " << i->first << " Value: " << i->second << '\n';
	return 0;
}

/**
 * Some main function to test shit.
 */
//int main(int a, char**params) {
//	// Reedb::getInstance();
//
//	Reedb::core()->terminate();
//
//	return 0;
//}
