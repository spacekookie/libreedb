/*
 * This file is part of Reedb and a test
 * 
 */

#include <libconfig.h++>
#include <iostream>

using namespace libconfig;

void doStuff() {
  /* Create config and dereference pointers & shit */
  Config cfg;

  /* Get root setting and add sub-settings */
  Setting &root = cfg.getRoot();
  
  root.add("awesome", Setting::TypeBoolean) = false;
  cfg.writeFile("test.cfg");
}

int main(int argc, char **argv) {
  doStuff();

  std::cout << "This is cool" << std::endl;
}
